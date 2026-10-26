#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "ComponentManager.h"
#include "Concert.h"
#include "ConcertArtist.h"
#include "ConcertRepository.h"
#include "Input.h"
#include "Logger.h"
#include "MenuView.h"
#include "ScreenManager.h"
#include "StatsManager.h"
#include "IStorageManager.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

ScreenManager::ScreenManager(ConcertRepository &repo, IStorageManager &storage, UIComponent &ui)
    : current_screen(Screen::Menu), repo(repo), storage(storage), ui(ui)
{
}

void ScreenManager::run()
{
    ui.menu_view.show_welcome();

    while (current_screen != Screen::Exit)
    {
        process_current_screen();
    }

    storage.save_artists(repo.get_artists());
    storage.save_concert_artists(repo.get_concert_artists());
    storage.save_concerts(repo.get_concerts());
}

void ScreenManager::process_current_screen()
{
    Screen previous_screen = current_screen;

    switch (current_screen)
    {
    case Screen::Menu:
        show_menu();
        break;
    case Screen::AddConcert:
    {
        std::map<std::string, ArtistRoles> artists;
        bool input_failed = false;
        while (true)
        {
            InputResult<std::string> artist_name = _get_validated_input<std::string, std::string>(
                "artist name", "Invalid artist name input",
                [&]() { return ui.input.get_string_input(); },
                [&](std::string &input) { return validator.validate_artist(input); });
            if (!artist_name.was_successful)
            {
                input_failed = true;
                break;
            }

            InputResult<ArtistRoles> artist_role = _get_validated_input<std::string, ArtistRoles>(
                "artist role [Headline, Support, Guest]", "Invalid artist role input",
                [&]() { return ui.input.get_string_input(); },
                [&](std::string &input) { return validator.validate_role(input); });
            if (!artist_role.was_successful)
            {
                input_failed = true;
                break;
            }
            artists.emplace(artist_name.value, artist_role.value);

            ui.pop_up_view.show_input_request("another artist [Y/N]");
            InputResult<bool> again = ui.input.get_bool_input();
            if (!again.was_successful)
            {
                input_failed = true;
                break;
            }
            if (!again.value)
                break;
        }
        if (input_failed)
        {
            current_screen = Screen::Menu;
            break;
        }

        InputResult<std::string> venue = _get_validated_input<std::string, std::string>(
            "venue name", "Invalid venue input", [&]() { return ui.input.get_string_input(); },
            [&](std::string &input) { return validator.validate_venue(input); });
        if (!venue.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }

        InputResult<std::string> city = _get_validated_input<std::string, std::string>(
            "city name", "Invalid city input", [&]() { return ui.input.get_string_input(); },
            [&](std::string &input) { return validator.validate_city(input); });
        if (!city.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }

        InputResult<std::string> date = _get_validated_input<std::string, std::string>(
            "date [DD-MM-YYYY]", "Invalid date input",
            [&]() { return ui.input.get_string_input(); },
            [&](std::string &input) { return validator.validate_date(input); });
        if (!date.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }

        InputResult<double> cost = _get_validated_input<double, double>(
            "cost (£)", "Invalid cost input",
            [&]() { return ui.input.get_numerical_input<double>(); },
            [&](double &input) { return validator.validate_cost(input); });
        if (!cost.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }

        repo.add_concert(artists, venue.value, city.value, date.value, cost.value);
        current_screen = Screen::Menu;
        break;
    }
    case Screen::ViewConcerts:
        ui.concerts_view.show(repo.get_concerts(), repo.get_artists(),
                              repo.get_concert_artists());
        current_screen = Screen::Menu;
        break;
    case Screen::DeleteConcert:
    {
        InputResult<int32_t> concert_id = _get_validated_input<int32_t, int32_t>(
            "concert ID", "Invalid concert ID input",
            [&]() { return ui.input.get_numerical_input<int32_t>(); },
            [&](int32_t &input) { return validator.validate_id(input, repo.get_concerts()); });
        if (!concert_id.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }
        repo.remove_concert(concert_id.value);
        current_screen = Screen::Menu;
        break;
    }
    case Screen::EditConcert:
    {
        InputResult<int32_t> concert_id = _get_validated_input<int32_t, int32_t>(
            "concert ID", "Invalid concert ID input",
            [&]() { return ui.input.get_numerical_input<int32_t>(); },
            [&](int32_t &input) { return validator.validate_id(input, repo.get_concerts()); });
        if (!concert_id.was_successful)
        {
            current_screen = Screen::Menu;
            break;
        }

        ui.menu_view.show_edit_menu();

        InputResult<int32_t> attribute_choice = ui.input.get_numerical_input<int32_t>();
        if (!attribute_choice.was_successful ||
            !validator.is_within_bounds(attribute_choice.value, 1, 5))
        {
            LOG_WARN("Invalid attribute selection");
            ui.pop_up_view.show_error_message("Invalid attribute selection");
            break;
        }

        switch (attribute_choice.value)
        {
        case 1:
        {
            auto concert_it = std::find_if(repo.get_concerts().begin(), repo.get_concerts().end(),
                                           [&](const Concert &concert) {
                                               return concert.get_concert_id() == concert_id.value;
                                           });
            if (concert_it == repo.get_concerts().end())
            {
                LOG_WARN("Concert ID not found");
                ui.pop_up_view.show_error_message("Concert ID not found");
                break;
            }

            ui.menu_view.show_edit_artist_options();
            InputResult<int32_t> raw_action = ui.input.get_numerical_input<int32_t>();
            if (!raw_action.was_successful || !validator.is_within_bounds(raw_action.value, 1, 3))
            {
                LOG_WARN("Invalid artist action selection");
                ui.pop_up_view.show_error_message("Invalid artist action selection");
                break;
            }

            if (raw_action.value == 1)
            {
                InputResult<std::string> artist_name =
                    _get_validated_input<std::string, std::string>(
                        "artist name", "Invalid artist name input",
                        [&]() { return ui.input.get_string_input(); },
                        [&](std::string &input) { return validator.validate_artist(input); });
                if (!artist_name.was_successful)
                    break;

                auto artist_it = std::find_if(repo.get_artists().begin(), repo.get_artists().end(),
                                              [&](const Artist &artist)
                                              { return artist.get_name() == artist_name.value; });

                if (artist_it != repo.get_artists().end())
                {
                    auto concert_artist_it =
                        std::find_if(repo.get_concert_artists().begin(),
                                     repo.get_concert_artists().end(),
                                     [&](const ConcertArtist &artist_concert)
                                     {
                                         return artist_concert.get_concert_id() == concert_id.value &&
                                                artist_concert.get_artist_id() == artist_it->get_id();
                                     });
                    if (concert_artist_it != repo.get_concert_artists().end())
                    {
                        LOG_WARN("Artist is already associated with this concert");
                        ui.pop_up_view.show_error_message(
                            "Artist is already associated with this concert");
                        break;
                    }
                }

                InputResult<ArtistRoles> artist_role =
                    _get_validated_input<std::string, ArtistRoles>(
                        "artist role [Headline, Support, Guest]", "Invalid artist role input",
                        [&]() { return ui.input.get_string_input(); },
                        [&](std::string &input) { return validator.validate_role(input); });
                if (!artist_role.was_successful)
                    break;
                repo.add_artist(concert_id.value, artist_name.value, artist_role.value);
                break;
            }

            std::vector<int32_t> current_artist_ids;
            for (const ConcertArtist &concert_artist : repo.get_concert_artists())
            {
                if (concert_artist.get_concert_id() == concert_id.value)
                {
                    current_artist_ids.push_back(concert_artist.get_artist_id());
                }
            }

            int32_t artist_id;
            if (!select_artist(current_artist_ids, artist_id))
            {
                break;
            }

            if (raw_action.value == 2)
            {
                std::vector<int32_t> replacement_choices;
                for (const Artist &artist : repo.get_artists())
                {
                    if (artist.get_id() == artist_id ||
                        std::find(current_artist_ids.begin(), current_artist_ids.end(),
                                  artist.get_id()) == current_artist_ids.end())
                    {
                        replacement_choices.push_back(artist.get_id());
                    }
                }
                int32_t replacement_artist_id;
                if (!select_artist(replacement_choices, replacement_artist_id))
                {
                    break;
                }

                ui.pop_up_view.show_input_request("artist role [Headline, Support, Guest]");
                InputResult<std::string> raw_role = ui.input.get_string_input();
                if (!raw_role.was_successful)
                {
                    ui.pop_up_view.show_error_message("Invalid artist role input");
                    break;
                }
                ValidationResult<ArtistRoles> role = validator.validate_role(raw_role.value);
                if (!role.is_valid)
                {
                    ui.pop_up_view.show_error_message(role.error_message);
                    break;
                }
                repo.edit_artist(concert_id.value, artist_id, replacement_artist_id, role.value);
                break;
            }

            repo.delete_artist(concert_id.value, artist_id);
            break;
        }
        case 2:
        {
            ui.pop_up_view.show_input_request("venue name");
            InputResult<std::string> raw_venue = ui.input.get_string_input();
            if (!raw_venue.was_successful)
            {
                LOG_WARN("Invalid venue input");
                ui.pop_up_view.show_error_message("Invalid venue input");
                break;
            }
            ValidationResult<std::string> validated_venue =
                validator.validate_venue(raw_venue.value);
            if (!validated_venue.is_valid)
            {
                LOG_WARN(validated_venue.error_message);
                ui.pop_up_view.show_error_message(validated_venue.error_message);
                break;
            }
            repo.update_venue(concert_id.value, validated_venue.value);
            break;
        }
        case 3:
        {
            ui.pop_up_view.show_input_request("city name");
            InputResult<std::string> raw_city = ui.input.get_string_input();
            if (!raw_city.was_successful)
            {
                LOG_WARN("Invalid city input");
                ui.pop_up_view.show_error_message("Invalid city input");
                break;
            }
            ValidationResult<std::string> validated_city = validator.validate_city(raw_city.value);
            if (!validated_city.is_valid)
            {
                LOG_WARN(validated_city.error_message);
                ui.pop_up_view.show_error_message(validated_city.error_message);
                break;
            }
            repo.update_city(concert_id.value, validated_city.value);
            break;
        }
        case 4:
        {
            ui.pop_up_view.show_input_request("date [DD-MM-YYYY]");
            InputResult<std::string> raw_date = ui.input.get_string_input();
            if (!raw_date.was_successful)
            {
                LOG_WARN("Invalid date input");
                ui.pop_up_view.show_error_message("Invalid date input");
                break;
            }
            ValidationResult<std::string> validated_date = validator.validate_date(raw_date.value);
            if (!validated_date.is_valid)
            {
                LOG_WARN(validated_date.error_message);
                ui.pop_up_view.show_error_message(validated_date.error_message);
                break;
            }
            repo.update_date(concert_id.value, validated_date.value);
            break;
        }
        case 5:
        {
            ui.pop_up_view.show_input_request("cost");
            InputResult<double> raw_cost = ui.input.get_numerical_input<double>();
            if (!raw_cost.was_successful)
            {
                LOG_WARN("Invalid cost input");
                ui.pop_up_view.show_error_message("Invalid cost input");
                break;
            }
            ValidationResult validated_cost = validator.validate_cost(raw_cost.value);
            if (!validated_cost.is_valid)
            {
                LOG_WARN(validated_cost.error_message);
                ui.pop_up_view.show_error_message(validated_cost.error_message);
                break;
            }
            repo.update_cost(concert_id.value, validated_cost.value);
            break;
        }
        default:
            LOG_WARN("Invalid attribute selection");
            break;
        }

        current_screen = Screen::Menu;
        break;
    }
    case Screen::ConcertStats:
        ui.stats_view.show_concert_stats(StatsManager::get_concert_stats(repo.get_concerts()));
        current_screen = Screen::Menu;
        break;
    case Screen::ArtistStats:
        for (const Artist &artist : repo.get_artists())
        {
            ui.stats_view.show_artist_stats(
                StatsManager::get_artist_stats(artist, repo.get_concerts(),
                                               repo.get_concert_artists()));
        }
        current_screen = Screen::Menu;
        break;
    default:
        std::string message = "Screen value is not recognised";
        LOG_ERROR(message);
        throw std::runtime_error(message);
    }

    if (previous_screen == current_screen)
    {
        LOG_INFO(enum_to_string(current_screen) + " screen initialised");
    }
    else
    {
        LOG_INFO("Screen changed from " + enum_to_string(previous_screen) + " to " +
                 enum_to_string(current_screen));
    }
}

bool ScreenManager::select_artist(const std::vector<int32_t> &choices, int32_t &selected_id)
{
    if (choices.empty())
    {
        ui.pop_up_view.show_error_message("No artists are available for this action");
        return false;
    }

    for (int32_t artist_id : choices)
    {
        for (const Artist &artist : repo.get_artists())
        {
            if (artist.get_id() == artist_id)
            {
                ui.pop_up_view.show_menu_option(std::to_string(artist_id) + ": " +
                                                artist.get_name());
                break;
            }
        }
    }

    ui.pop_up_view.show_input_request("artist ID");
    InputResult<int32_t> raw_artist_id = ui.input.get_numerical_input<int32_t>();
    if (!raw_artist_id.was_successful ||
        std::find(choices.begin(), choices.end(), raw_artist_id.value) == choices.end())
    {
        ui.pop_up_view.show_error_message("Invalid artist ID");
        return false;
    }

    selected_id = raw_artist_id.value;
    return true;
}

void ScreenManager::show_menu()
{
    ui.menu_view.show_main_menu();

    InputResult<int32_t> user_input = ui.input.get_numerical_input<int32_t>();

    if (!user_input.was_successful || user_input.value < 1 || user_input.value > 7)
    {
        ui.pop_up_view.show_error_message("Invalid input");
        return;
    }

    switch (user_input.value)
    {
    case 1:
        current_screen = Screen::AddConcert;
        break;
    case 2:
        current_screen = Screen::ViewConcerts;
        break;
    case 3:
        current_screen = Screen::DeleteConcert;
        break;
    case 4:
        current_screen = Screen::EditConcert;
        break;
    case 5:
        current_screen = Screen::ConcertStats;
        break;
    case 6:
        current_screen = Screen::ArtistStats;
        break;
    case 7:
        current_screen = Screen::Exit;
        break;
    }
}

std::string ScreenManager::enum_to_string(Screen screen)
{
    switch (screen)
    {
    case Screen::Menu:
    {
        return "menu";
    }
    case Screen::AddConcert:
    {
        return "add concert";
    }
    case Screen::ViewConcerts:
    {
        return "view concerts";
    }
    case Screen::DeleteConcert:
    {
        return "delete concert";
    }
    case Screen::EditConcert:
    {
        return "edit concert";
    }
    case Screen::ConcertStats:
    {
        return "concert stats";
    }
    case Screen::ArtistStats:
    {
        return "artist stats";
    }
    case Screen::Exit:
    {
        return "exit";
    }
    default:
    {
        return "Error";
    }
    }
}

template <typename InputT, typename T>
InputResult<T> ScreenManager::_get_validated_input(
    const std::string &prompt, const std::string &invalid_input_message,
    std::function<InputResult<InputT>()> get_input,
    std::function<ValidationResult<T>(InputT &)> validate)
{
    ui.pop_up_view.show_input_request(prompt);
    InputResult<InputT> raw_input = get_input();
    if (!raw_input.was_successful)
    {
        LOG_WARN(invalid_input_message);
        ui.pop_up_view.show_error_message(invalid_input_message);
        return {T{}, false};
    }

    ValidationResult<T> result = validate(raw_input.value);
    if (!result.is_valid)
    {
        LOG_WARN(result.error_message);
        ui.pop_up_view.show_error_message(result.error_message);
        return {T{}, false};
    }

    return {result.value, true};
}
