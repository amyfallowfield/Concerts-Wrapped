#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "InputManager.h"
#include "Logger.h"
#include "Performance.h"
#include "Utilities.h"
#include "ValidationManager.h"

ConcertRepository::ConcertRepository(StorageManager& storage)
{
    artists = storage.load<Artist>();
    concerts = storage.load<Concert>();
    performances = storage.load<Performance>();
}

void ConcertRepository::add()
{
    Concert new_concert = create_concert();
    update_performances(new_concert);

    concerts.push_back(new_concert);
    _refresh_artists(new_concert);

    Logger::Info("ConcertRepository", "add", "Concert created successfully");
}

void ConcertRepository::remove()
{
    int32_t id = get_concert_id();
    Concert deleted_concert = _get_concert_from_id(id);

    auto performance_it = std::find_if(
        performances.begin(), performances.end(),
        [&](const Performance& performance)
        {
           return performance.get_artist() == deleted_concert.get_artist();
        }
    );

    if (performance_it == performances.end())
    {
        artists.erase(
            std::remove_if(
                artists.begin(), artists.end(),
                [&](const Artist& artist)
                {
                    return artist.get_name() == deleted_concert.get_artist();
                }
            ),
            artists.end()
        );
    }

    auto concert_it = std::find_if(
        concerts.begin(), concerts.end(),
        [&](const Concert& concert)
        {
            return concert.get_id() == deleted_concert.get_id();
        }
    );

    if (concert_it != concerts.end())
    {
        concerts.erase(concert_it);
    }

    performances.erase(
        std::remove_if(
            performances.begin(), performances.end(),
            [&](const Performance& performance)
            {
                return performance.get_show_id() == deleted_concert.get_id();
            }
        ),
        performances.end()
    );

    _refresh_artists(deleted_concert);
}

void ConcertRepository::edit()
{
    int32_t id = get_concert_id();
    Concert& concert = _get_concert_from_id(id);

    int input = input_manager.select_attribute();

    switch(input)
    {
    case 1:
    {
        std::string artist = 
            input_manager.get_attribute_input<std::string>(
                "Artist Name: ",
                [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_artist(input); });
        concert.set_artist(artist);
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
        concert.set_venue(venue);
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
        concert.set_city(city);
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
        concert.set_date(date);
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
        concert.set_cost(cost_as_int);
        break;
    }
    default:
        throw std::runtime_error("Cannot update attribute not owned by concert model");
    }

    _refresh_artists(concert);
}

void ConcertRepository::print()
{
    for (const Concert& concert : concerts)
    {
        concert.print();
        std::cout << '\n';
    }
}

std::vector<Artist> ConcertRepository::get_artists() { return artists; }
std::vector<Concert> ConcertRepository::get_concerts() { return concerts; }
std::vector<Performance> ConcertRepository::get_performances() { return performances; }

Concert ConcertRepository::create_concert()
{
    std::string artist = 
        input_manager.get_attribute_input<std::string>(
            "Artist Name: ",
            [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_artist(input); });

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

    std::vector<std::string> supports {};
    int count {1};

    while (true)
    {
        if (input_manager.get_bool_input("Add support act? [Y/N] "))
        {
            std::string support =
                input_manager.get_attribute_input<std::string>(
                    "Support #" + std::to_string(count) + ": ",
                    [&](const std::string& prompt) { return input_manager.get_string_input(prompt); },
                    [&](std::string& input) { return input; },
                    [&](std::string& input) { return validator.validate_artist(input); });

            supports.push_back(support);
            count++;
        }
        else { break; }
    }

    return {artist, venue, city, date, cost_as_int, supports};
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
    std::vector<std::string> concert_artists{};
    concert_artists.push_back(concert.get_artist());
    for (auto e : concert_artists)
    {
        std::cout << e;
    }
    concert_artists.insert(concert_artists.end(), concert.get_supports().begin(), concert.get_supports().end());

    for (std::string artist_name : concert_artists)
    {
        auto it = std::find_if(
            artists.begin(), artists.end(),
            [&](const Artist& artist)
            {
                return artist.get_name() == artist_name;
            }
        );
        if (it != artists.end())
        {
            artists.erase(it);
        }

        std::vector<Concert> artists_concerts{};
        for (const Concert& concert : concerts)
        {
            if (concert.get_artist() == artist_name ||
                std::find(concert.get_supports().begin(),
                concert.get_supports().end(),
                artist_name)
                != concert.get_supports().end());
            {
                artists_concerts.push_back(concert);
            }
        }

        if (artists_concerts.size() != 0)
        {
            artists.push_back(Artist{artist_name, artists_concerts});
        }
    }
}

void ConcertRepository::update_performances(const Concert& new_concert)
{
    performances.push_back(Performance(new_concert.get_id(), new_concert.get_artist(), "Headliner"));

    std::vector<std::string> supports = new_concert.get_supports();
    for (std::string support : supports)
    {
        performances.push_back(Performance(new_concert.get_id(), support, "Support"));
    }
}

Concert& ConcertRepository::_get_concert_from_id(int32_t id)
{
    auto it = std::find_if(
        concerts.begin(), concerts.end(),
        [&](const Concert& concert)
        {
            return concert.get_id() == id;
        }
    );

    return *it;
}
