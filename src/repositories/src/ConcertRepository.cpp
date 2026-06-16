#include <iostream>
#include <string>

#include "../../services/include/Logger.h"
#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
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
    std::string artist = get_input("Artist Name: ", [&](std::string& input) { return validator.validate_artist(input); });
    std::string venue = get_input("Venue Name: ", [&](std::string& input) { return validator.validate_venue(input); });
    std::string city = get_input("City: ", [&](std::string& input) { return validator.validate_city(input); });
    std::string date = get_input("Date [Format: DD-MM-YYYY]: ", [&](std::string& input) { return validator.validate_date(input); });
    int32_t cost = get_monetary_input("Cost: £", [&](int32_t input) { return validator.validate_cost(input); });
    
    return {artist, venue, city, date, cost};

}

void ConcertRepository::update_artists(const Concert& new_concert)
{
    auto artist_it = std::find_if(artists.begin(), artists.end(),
        [&](const Artist& existing_artist) {
            return existing_artist.name == new_concert.get_artist();
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

std::string ConcertRepository::get_input(std::string prompt, std::function<ValidationResult<std::string>(std::string&)> validation_method)
{
    std::string input;

    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        ValidationResult<std::string> result = validation_method(input);

        if (result.is_valid) { return result.value; }

        std::cout << result.error_message;
        Logger::Warn("ConcertRepository", "get_input", result.error_message);
    }
}

int32_t ConcertRepository::get_monetary_input(std::string prompt, std::function<ValidationResult<int32_t>(int32_t)> validation_method)
{
    double input;

    while (true)
    {
        std::cout << prompt;

        if (!Utilities::parse_float(input))
            continue;

        input = static_cast<int32_t>(input * 100);

        ValidationResult<int32_t> result = validation_method(input);

        if (result.is_valid) { return result.value; }

        std::cout << result.error_message;
        Logger::Warn("ConcertRepository", "get_monetary_input", result.error_message);
    }
}