#include <iostream>

#include "models/Concert.h"
#include "ConcertRepository.h"
#include "Utilities.h"

void ConcertRepository::add()
{
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;

    std::cout << "Artist Name: ";
    std::getline(std::cin, artist);

    std::cout << "Venue Name: ";
    std::getline(std::cin, venue);

    std::cout << "City: ";
    std::getline(std::cin, city);

    std::cout << "Date [Format: DD-MM-YYYY]: ";
    std::getline(std::cin, date);

    while (true)
    {
        double input;

        std::cout << "Cost: £";

        if (!Utilities::parse_float(input) || input < 0)
            continue;

        cost = static_cast<int32_t>(input * 100);

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