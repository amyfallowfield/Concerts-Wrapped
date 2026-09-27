#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "InputManager.h"
#include "Logger.h"
#include "Utilities.h"
#include "ValidationManager.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

ConcertRepository::ConcertRepository(StorageManager& storage)
{
    artists = storage.load<Artist>();
    concerts = storage.load<Concert>();
}

void ConcertRepository::add()
{
    Concert new_concert = create_concert();

    concerts.push_back(new_concert);
    _refresh_artists(new_concert);

    LOG_INFO("Concert created successfully");
}

void ConcertRepository::remove()
{
    int32_t id = get_concert_id();
    auto deleted_concert_it = _find_concert_by_id(id);

    if (deleted_concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }

    Concert deleted_concert = *deleted_concert_it;
    concerts.erase(deleted_concert_it);
    _refresh_artists(deleted_concert);

    LOG_INFO("Concert deleted successfully");
}

void ConcertRepository::edit()
{
    int32_t id = get_concert_id();
    auto concert_it = _find_concert_by_id(id);

    int input = input_manager.select_attribute();

    switch(input)
    {
    case 1:
    {
        std::vector<std::pair<int32_t, std::string>> artist_id_name_pair {};
        for (const auto& [artist_id, role] : concert_it->get_artists())
        {
            auto artist_it = _find_artist_by_id(artist_id);

            if (artist_it == artists.end())
            {
                LOG_ERROR("Artist ID not found");
                return;
            }

            artist_id_name_pair.push_back(std::pair(artist_id, artist_it->get_name()));
        }

        ArtistUpdateRequest artist_data = input_manager.get_artist_update_data(artist_id_name_pair);

        if (!artist_data.success)
        {
            LOG_ERROR("Invalid input when selecting artist modification request");
            return;
        }

        switch (artist_data.action)
        {
        case Actions::Add:
        {
            std::pair<int32_t, ArtistRoles> artist_id_role_pair = _get_new_artist_id_role();

            concert_it->add_artist(artist_id_role_pair.first, artist_id_role_pair.second);

            LOG_INFO("Artist added successfully");
            break;
        }
        case Actions::Edit:
        {
            std::pair<int32_t, ArtistRoles> artist_id_role_pair = _get_new_artist_id_role();
            int32_t old_id = artist_data.artist_id;

            Concert old_concert_id = *concert_it;
            concert_it->edit_artist(old_id, artist_id_role_pair.first, artist_id_role_pair.second);
            _refresh_artists(old_concert_id);

            LOG_INFO("Artist editted successfully");
            break;
        }
        case Actions::Delete:
        {
            Concert old_concert_id = *concert_it;
            concert_it->delete_artist(artist_data.artist_id);
            _refresh_artists(old_concert_id);

            LOG_INFO("Artist deleted successfully");
            break;
        }
        default:
            LOG_ERROR("Invalid artist list modification action selection");
            return;
        }
        break;
    }
    case 2:
    {
        std::string venue = 
            input_manager.get_attribute_input<std::string>(
                "Venue Name: ",
                [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_venue(input); });
        concert_it->set_venue(venue);
        break;
    }
    case 3:
    {
        std::string city = 
            input_manager.get_attribute_input<std::string>(
                "City: ",
                [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_city(input); });
        concert_it->set_city(city);
        break;
    }
    case 4:
    {
        std::string date = 
            input_manager.get_attribute_input<std::string>(
                "Date [Format: DD-MM-YYYY]: ",
                [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_date(input); });
        concert_it->set_date(date);
        break;
    }
    case 5:
    {
        double cost = 
            input_manager.get_attribute_input<double>(
                "Cost: £",
                [&](const std::string& prompt) { return input_manager.get_decimal_input(prompt); },
                [&](double& input) { return input * 100; },
                [&](double& input) { return validator.validate_cost(input); });
        int32_t cost_as_int = static_cast<int32_t>(cost);
        concert_it->set_cost(cost_as_int);
        break;
    }
    default:
        LOG_ERROR("Invalid concert attribute selection");
        return;
    }

    _refresh_artists(*concert_it);

    LOG_INFO("Concert editted successfully");
}

void ConcertRepository::print()
{
    for (const Concert& concert : concerts)
    {
        std::vector<std::pair<std::string, ArtistRoles>> artist_name_role_pair;
        for (const auto& [artist_id, role] : concert.get_artists())
        {
            auto it = _find_artist_by_id(artist_id);

            if (it != artists.end())
            {
                artist_name_role_pair.emplace_back(it->get_name(), role);
            }
        }

        concert.print(artist_name_role_pair);
        std::cout << '\n';
    }
}

const std::vector<Artist>&  ConcertRepository::get_artists() const { return artists; }
const std::vector<Concert>&  ConcertRepository::get_concerts() const { return concerts; }

Concert ConcertRepository::create_concert()
{
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    while (true)
    {
        std::string name = 
            input_manager.get_attribute_input<std::string>(
                "Artist Name: ",
                [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_artist(input); });

        ArtistRoles role = 
            input_manager.get_attribute_input<ArtistRoles>(
                "New artist role [Headline, Support, Guest]: ",
                [&](const std::string& prompt) { return input_manager.get_role_input(prompt); },
                [&](ArtistRoles& input) { return input; },
                [&](ArtistRoles& input) { return validator.validate_role(input); });

        auto artist_it = _find_artist_by_name(name);

        int32_t id;
        if (artist_it == artists.end())
        {
            Artist artist = Artist(name);
            artists.push_back(artist);
            id = artist.get_id();
        }
        else
        {
            id = artist_it->get_id();
        }

        artist_id_role_map.insert_or_assign(id, role);

        if (!input_manager.get_bool_input("Add another artist? [Y/N] ")) { break; }
    }

    std::string venue = 
        input_manager.get_attribute_input<std::string>(
            "Venue Name: ",
            [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_venue(input); });

    std::string city = 
        input_manager.get_attribute_input<std::string>(
            "City: ",
            [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_city(input); });

    std::string date = 
        input_manager.get_attribute_input<std::string>(
            "Date [Format: DD-MM-YYYY]: ",
            [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_date(input); });

    double cost = 
        input_manager.get_attribute_input<double>(
            "Cost: £",
            [&](const std::string& prompt) { return input_manager.get_decimal_input(prompt); },
            [&](double& input) { return input * 100; },
            [&](double& input) { return validator.validate_cost(input); });
    int32_t cost_as_int = static_cast<int32_t>(cost);

    return {artist_id_role_map, venue, city, date, cost_as_int};
}

int32_t ConcertRepository::get_concert_id()
{
    int32_t id = 
        input_manager.get_attribute_input<int32_t>(
            "ID: ",
            [&](const std::string& prompt) { return input_manager.get_numerical_input(prompt); },
            [&](int32_t& input) { return input; },
            [&](int32_t& input) { return validator.validate_id(input, concerts); });

    return id;
}

void ConcertRepository::_refresh_artists(const Concert& concert)
{
    for (const auto& artist : concert.get_artists())
    {
        int32_t artist_id = artist.first;
        auto artist_it = _find_artist_by_id(artist_id);

        if (artist_it == artists.end())
        {
            LOG_ERROR("Concert contains invalid artist ID");
            continue;
        }

        bool artist_still_used = std::any_of(
            concerts.begin(), concerts.end(),
            [&](const Concert& concert)
            {
                const auto& concert_artists = concert.get_artists();
                return concert_artists.find(artist_id) != concert_artists.end();
            }
        );

        if (!artist_still_used)
        {
            artists.erase(artist_it);
        }

        LOG_INFO("Artists successfully updated");
    }
}

std::vector<Concert>::iterator ConcertRepository::_find_concert_by_id(int32_t id)
{
    auto it = std::find_if(
        concerts.begin(), concerts.end(),
        [&](const Concert& concert)
        {
            return concert.get_concert_id() == id;
        }
    );

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_id(int32_t id)
{
    auto it = std::find_if(
        artists.begin(), artists.end(),
        [&](const Artist& artist)
        {
            return artist.get_id() == id;
        }
    );

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_name(const std::string& name)
{
    auto it = std::find_if(
        artists.begin(), artists.end(),
        [&](const Artist& artist)
        {
            return artist.get_name() == name;
        }
    );

    return it;
}

std::pair<int32_t, ArtistRoles> ConcertRepository::_get_new_artist_id_role()
{
    std::string name = 
        input_manager.get_attribute_input<std::string>(
            "New artist name: ",
            [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_artist(input); });
    ArtistRoles role = 
        input_manager.get_attribute_input<ArtistRoles>(
            "New artist role [Headline, Support, Guest]: ",
            [&](const std::string& prompt) { return input_manager.get_role_input(prompt); },
            [&](ArtistRoles& input) { return input; },
            [&](ArtistRoles& input) { return validator.validate_role(input); });

    auto artist_it = _find_artist_by_name(name);

    int32_t id;
    if (artist_it == artists.end())
    {
        Artist artist = Artist(name);
        artists.push_back(artist);
        id = artist.get_id();
    }
    else
    {
        id = artist_it->get_id();
    }

    return {id, role};
}