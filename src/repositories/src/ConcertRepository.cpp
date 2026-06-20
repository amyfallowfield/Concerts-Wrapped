#include <iostream>
#include <string>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "Logger.h"
#include "StorageManager.h"
#include "Utilities.h"
#include "ValidationManager.h"

ConcertRepository::ConcertRepository()
{
    StorageManager storage = StorageManager();
    concerts = storage.load<Concert>();
    artists = storage.load<Artist>();
}

void ConcertRepository::add()
{
    Concert new_concert = create_concert();
    update_artists(new_concert);

    concerts.push_back(new_concert);

    Logger::Info("ConcertRepository", "add", "Concert created successfully");
}

void ConcertRepository::print()
{
    for (const Concert& concert : concerts)
    {
        concert.print();
        std::cout << '\n';
    }
}

std::vector<Concert> ConcertRepository::get_concerts() { return concerts; }
std::vector<Artist> ConcertRepository::get_artists() { return artists; }

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

    int32_t cost = 
        get_input<int32_t>(
            "Cost: £",
            [&](const std::string& prompt) { return _get_monetary_input(prompt); },
            [&](int32_t& input) { return static_cast<int32_t>(input * 100);; },
            [&](int32_t& input) { return validator.validate_cost(input); });
    
    return {artist, venue, city, date, cost};
}

void ConcertRepository::update_artists(const Concert& new_concert)
{
    auto artist_it = std::find_if(artists.begin(), artists.end(),
        [&](const Artist& existing_artist) {
            return existing_artist.get_name() == new_concert.get_artist();
    });

    if (artist_it == artists.end())
    {
        artists.push_back(Artist{new_concert.get_artist(), new_concert.get_date(), new_concert.get_cost()});
        Logger::Info("ConcertRepository", "update_artists", "Artist created successfully");
    }
    else
    {
        Artist& current_artist = *artist_it;
        current_artist.update(new_concert.get_date(), new_concert.get_cost());
        Logger::Info("ConcertRepository", "update_artists", "Artist update successfully");
    }
}

std::string ConcertRepository::_get_string_input(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input;
}

int32_t ConcertRepository::_get_monetary_input(std::string prompt)
{
    double input;

    std::cout << prompt;
    
    if (!Utilities::parse_float(input))
        return -1;

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

        if (result.is_valid) { return result.value; }

        std::cout << result.error_message;
        Logger::Warn("ConcertRepository", "get_input", result.error_message);
    }
}
