#include <iostream>
#include <nlohmann/json.hpp>

#include "models/Concert.h"

using json = nlohmann::json;

Concert::Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost)
    : artist(artist),
      venue(venue),
      city(city),
      date(date),
      cost(cost)
{}

Concert::Concert(const json& data)
    : artist(data.at("artist")),
      venue(data.at("venue")),
      city(data.at("city")),
      date(data.at("date")),
      cost(data.at("cost"))
{}

void Concert::print() const
{
    std::cout << "Artist: " << artist << '\n';
    std::cout << "Venue: " << venue << '\n';
    std::cout << "City: " << city << '\n';
    std::cout << "Date: " << date << '\n';
    std::cout << "Cost: £" << cost << '\n';
}

json Concert::to_json() const
{
    return json{
        {"artist", artist},
        {"venue", venue},
        {"city", city},
        {"date", date},
        {"cost", cost}
    };
}