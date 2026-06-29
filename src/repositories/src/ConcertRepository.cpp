#include <algorithm>
#include <iostream>
#include <string>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "Logger.h"
#include "Performance.h"
#include "Utilities.h"
#include "ValidationManager.h"

ConcertRepository::ConcertRepository()
{
    artists = storage.load<Artist>();
    concerts = storage.load<Concert>();
    performances = storage.load<Performance>();
}

void ConcertRepository::add()
{
    Concert new_concert = create_concert();
    update_artists(new_concert);
    update_performances(new_concert);

    concerts.push_back(new_concert);

    Logger::Info("ConcertRepository", "add", "Concert created successfully");
}

void ConcertRepository::remove()
{
    int32_t id = get_concert_id();
    Concert deleted_concert = _get_concert_from_id(id);

    auto it = std::find_if(
        performances.begin(), performances.end(),
        [&](const Performance& performance)
        {
           return performance.get_artist() == deleted_concert.get_artist();
        }
    );

    if (it == performances.end())
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

    concerts.erase(
        std::remove_if(
            concerts.begin(), concerts.end(),
            [&](const Concert& concert)
            {
                return concert.get_id() == deleted_concert.get_id();
            }
        ),
        concerts.end()
    );

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
}

void ConcertRepository::edit()
{
    int32_t id = get_concert_id();
    Concert concert = _get_concert_from_id(id);

    int input;

    std::cout << "1. Artist\n";
    std::cout << "2. Venue\n";
    std::cout << "3. City\n";
    std::cout << "4. Date\n";
    std::cout << "5. Cost\n";
    std::cout << "Selection: ";

    if (!Utilities::parse_int(input) || input < 1 || input > 5)
    {
        std::cout << '\n';
        return;
    }

    std::cout << '\n';

    switch(input)
    {
    case 1:
    {
        std::string artist = 
            get_input<std::string>(
                "Artist Name: ",
                [&](const std::string& prompt) { return _get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_artist(input); });
        concert.set_artist(artist);
        break;
    }
    case 2:
    {
        std::string venue = 
            get_input<std::string>(
                "Venue Name: ",
                [&](const std::string& prompt) { return _get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_venue(input); });
        concert.set_venue(venue);
        break;
    }case 3:

    {
        std::string city = 
            get_input<std::string>(
                "City: ",
                [&](const std::string& prompt) { return _get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_city(input); });
        concert.set_city(city);
        break;
    }
    case 4:
    {
        std::string date = 
            get_input<std::string>(
                "Date [Format: DD-MM-YYYY]: ",
                [&](const std::string& prompt) { return _get_string_input(prompt); },
                [&](std::string& input) { return input; },
                [&](std::string& input) { return validator.validate_date(input); });
        concert.set_date(date);
        break;
    }
    case 5:
    {
        double cost = 
            get_input<double>(
                "Cost: £",
                [&](const std::string& prompt) { return _get_decimal_input(prompt); },
                [&](double& input) { return input * 100; },
                [&](double& input) { return validator.validate_cost(input); });
        int32_t cost_as_int = static_cast<int32_t>(cost);
        concert.set_cost(cost_as_int);
        break;
    }
    default:
        throw std::runtime_error("Cannot update attribute not owned by concert model");
    }
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
        get_input<std::string>(
            "Artist Name: ",
            [&](const std::string& prompt) { return _get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_artist(input); });

    std::string venue = 
        get_input<std::string>(
            "Venue Name: ",
            [&](const std::string& prompt) { return _get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_venue(input); });

    std::string city = 
        get_input<std::string>(
            "City: ",
            [&](const std::string& prompt) { return _get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_city(input); });

    std::string date = 
        get_input<std::string>(
            "Date [Format: DD-MM-YYYY]: ",
            [&](const std::string& prompt) { return _get_string_input(prompt); },
            [&](std::string& input) { return input; },
            [&](std::string& input) { return validator.validate_date(input); });

    double cost = 
        get_input<double>(
            "Cost: £",
            [&](const std::string& prompt) { return _get_decimal_input(prompt); },
            [&](double& input) { return input * 100; },
            [&](double& input) { return validator.validate_cost(input); });
    int32_t cost_as_int = static_cast<int32_t>(cost);

    return {artist, venue, city, date, cost_as_int};
}

int32_t ConcertRepository::get_concert_id()
{
    int32_t id = 
        get_input<int32_t>(
            "ID: ",
            [&](const std::string& prompt) { return _get_numerical_input(prompt); },
            [&](int32_t& input) { return input; },
            [&](int32_t& input) { return validator.validate_id(input, concerts); });

    return id;
}

void ConcertRepository::update_artists(const Concert& new_concert)
{
    auto artist_it = std::find_if(artists.begin(), artists.end(),
        [&](const Artist& existing_artist) {
            return existing_artist.get_name() == new_concert.get_artist();
    });

    if (artist_it == artists.end())
    {
        artists.push_back(Artist{new_concert});
        Logger::Info("ConcertRepository", "update_artists", "Artist created successfully");
    }
    else
    {
        Artist& current_artist = *artist_it;
        current_artist.update(new_concert);
        Logger::Info("ConcertRepository", "update_artists", "Artist update successfully");
    }
}

void ConcertRepository::update_performances(const Concert& new_concert)
{
    // Hardcoded value will be updated once roles are implemented
    performances.push_back(Performance(new_concert.get_id(), new_concert.get_artist(), "Headliner"));
}

Concert ConcertRepository::_get_concert_from_id(int32_t id)
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

std::string ConcertRepository::_get_string_input(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input;
}

int32_t ConcertRepository::_get_numerical_input(std::string prompt)
{
    int32_t input;

    std::cout << prompt;
    
    if (!Utilities::parse_int(input))
        return -1;

    return input;
}

double ConcertRepository::_get_decimal_input(std::string prompt)
{
    double input;

    std::cout << prompt;

    if (!Utilities::parse_float(input))
        return -1.0;

    return input;
}

template <typename T>
T ConcertRepository::get_input(
    const std::string& prompt,
    std::function<T(const std::string&)> input_method,
    std::function<T(T&)> transformation_method,
    std::function<ValidationResult<T>(T&)> validation_method)
{
    while (true)
    {
        T input = input_method(prompt);

        ValidationResult<T> result = validation_method(input);

        if (result.is_valid) { return transformation_method(result.value); }

        std::cout << result.error_message;
        Logger::Warn("ConcertRepository", "get_input", result.error_message);
    }
}
