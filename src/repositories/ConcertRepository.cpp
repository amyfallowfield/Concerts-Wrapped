#include <iostream>

#include "models/Artist.h"
#include "models/Concert.h"
#include "services/StorageManager.h"
#include "services/ValidationManager.h"
#include "ConcertRepository.h"
#include "Utilities.h"

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
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;

    while (true)
    {
        std::cout << "Artist Name: ";
        std::getline(std::cin, artist);

        ValidationResult result = validator.validate_artist(artist);
        if (result.is_valid) { break; }

        std::cout << result.error_message;
    }

    while (true)
    {
        std::cout << "Venue Name: ";
        std::getline(std::cin, venue);

        ValidationResult result = validator.validate_venue(venue);
        if (result.is_valid) { break; }

        std::cout << result.error_message;
    }

    while (true)
    {
        std::cout << "City: ";
        std::getline(std::cin, city);

        ValidationResult result = validator.validate_city(city);
        if (result.is_valid) { break; }

        std::cout << result.error_message;
    }

    while (true)
    {
        std::cout << "Date [Format: DD-MM-YYYY]: ";
        std::getline(std::cin, date);

        ValidationResult result = validator.validate_date(date);
        if (result.is_valid) { break; }

        std::cout << result.error_message;
    }

    while (true)
    {
        double input;

        std::cout << "Cost: £";

        if (!Utilities::parse_float(input))
            continue;

        cost = static_cast<int32_t>(input * 100);
        validator.validate_cost(cost).value;

        break;
    }

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
    }
    else
    {
        Artist& current_artist = *artist_it;
        current_artist.update(new_concert.get_date(), new_concert.get_cost());
    }
}