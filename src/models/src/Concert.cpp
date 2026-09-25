#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Concert.h"

using json = nlohmann::json;

int32_t Concert::_next_id = 1;

Concert::Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost, std::vector<std::string> supports)
    : id(_next_id),
      artist(artist),
      venue(venue),
      city(city),
      date(date),
      cost(cost),
      supports(supports)
{
    _next_id = id > _next_id ? ++id : ++_next_id;
}

Concert::Concert(const json& data)
    : id(data.at("id")),
      artist(data.at("artist")),
      venue(data.at("venue")),
      city(data.at("city")),
      date(data.at("date")),
      cost(data.at("cost")),
      supports(data.at("supports").get<std::vector<std::string>>())
{
    _next_id = id > _next_id ? id + 1 : ++_next_id;
}

void Concert::print() const
{
    std::cout << "ID: " << id << '\n';
    std::cout << "Artist: " << artist << '\n';
    std::cout << "Venue: " << venue << '\n';
    std::cout << "City: " << city << '\n';
    std::cout << "Date: " << date << '\n';
    std::cout << "Cost: £" << cost / 100.0 << '\n';
    std::cout << "Supports: \n";
    for (std::string support : supports)
    {
        std::cout << support << '\n';
    }
}

json Concert::to_json() const
{
    return json{
        {"id", id},
        {"artist", artist},
        {"venue", venue},
        {"city", city},
        {"date", date},
        {"cost", cost},
        {"supports", supports}
    };
}

bool Concert::operator==(const Concert& other) const
{
    return id == other.get_id();
}

int32_t Concert::get_id() const { return id; }
std::string Concert::get_artist() const { return artist; }
std::string Concert::get_venue() const { return venue; }
std::string Concert::get_city() const { return city; }
std::string Concert::get_date() const { return date; }
int32_t Concert::get_cost() const { return cost; }
const std::vector<std::string>& Concert::get_supports() const { return supports; }

void Concert::set_artist(std::string input) { artist = input; }
void Concert::set_venue(std::string input) { venue = input; }
void Concert::set_city(std::string input) { city = input; }
void Concert::set_date(std::string input) { date = input; }
void Concert::set_cost(int32_t input) { cost = input; }
void Concert::set_supports(std::vector<std::string> input) { supports = input; }

void Concert::add_support(const std::string& support)
{
    supports.push_back(support);
}

void Concert::edit_support(int index, const std::string& support)
{
    supports.at(index) = support;
}

void Concert::delete_support(int index)
{
    supports.erase(supports.begin() + static_cast<std::ptrdiff_t>(index));
}

void Concert::reset()
{
    _next_id = 1;
}
