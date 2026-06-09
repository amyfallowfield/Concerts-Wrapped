#include <iostream>

#include "models/Concert.h"
#include "services/ValidationManager.h"
#include "ConcertRepository.h"
#include "Utilities.h"

void ConcertRepository::add()
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

    concerts.push_back(Concert{artist, venue, city, date, cost});
}

void ConcertRepository::print()
{
    for (const Concert& concert : concerts)
    {
        concert.print();
        std::cout << '\n';
    }
}

std::vector<Concert> ConcertRepository::get_all()
{
    return concerts;
}