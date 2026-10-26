#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Concert.h"

using json = nlohmann::json;

int32_t Concert::_next_id = 1;

Concert::Concert(std::string venue, std::string city, std::string date, int32_t cost)
    : concert_id(_next_id), venue(venue), city(city), date(date), cost(cost)
{
    _next_id = concert_id > _next_id ? ++concert_id : ++_next_id;
}

Concert::Concert(const json &data)
    : concert_id(data.at("concert_id")),
      venue(data.at("venue")), city(data.at("city")),
      date(data.at("event_date")), cost(data.at("cost"))
{
    _next_id = concert_id > _next_id ? concert_id + 1 : ++_next_id;
}

json Concert::to_json() const
{
    return json{{"concert_id", concert_id},
                {"venue", venue},
                {"city", city},
                {"event_date", date},
                {"cost", cost}};
}

bool Concert::operator==(const Concert &other) const
{
    return concert_id == other.get_concert_id();
}

int32_t Concert::get_concert_id() const
{
    return concert_id;
}
std::string Concert::get_venue() const
{
    return venue;
}
std::string Concert::get_city() const
{
    return city;
}
std::string Concert::get_date() const
{
    return date;
}
int32_t Concert::get_cost() const
{
    return cost;
}

void Concert::set_venue(std::string input)
{
    venue = input;
}
void Concert::set_city(std::string input)
{
    city = input;
}
void Concert::set_date(std::string input)
{
    date = input;
}
void Concert::set_cost(int32_t input)
{
    cost = input;
}

void Concert::reset()
{
    _next_id = 1;
}
