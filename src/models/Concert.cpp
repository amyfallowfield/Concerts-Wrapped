#include <iostream>

#include "models/Concert.h"

Concert::Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost)
    : artist(artist), venue(venue), city(city), date(date), cost(cost)
{}

void Concert::print() const
{
    std::cout << "Artist: " << artist << '\n';
    std::cout << "Venue: " << venue << '\n';
    std::cout << "City: " << city << '\n';
    std::cout << "Date: " << date << '\n';
    std::cout << "Cost: £" << cost << '\n';
}