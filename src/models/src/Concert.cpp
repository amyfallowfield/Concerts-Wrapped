#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Concert.h"

using json = nlohmann::json;

int32_t Concert::_next_id = 1;

Concert::Concert(const std::unordered_map<int32_t, ArtistRoles>& artists, std::string venue, std::string city, std::string date, int32_t cost)
    : concert_id(_next_id),
      artists(artists),
      venue(venue),
      city(city),
      date(date),
      cost(cost)
{
    _next_id = concert_id > _next_id ? ++concert_id : ++_next_id;
}

Concert::Concert(const json& data)
    : concert_id(data.at("concert_id")),
      artists(data.at("artists").get<std::unordered_map<int32_t, ArtistRoles>>()),
      venue(data.at("venue")),
      city(data.at("city")),
      date(data.at("date")),
      cost(data.at("cost"))
{
    _next_id = concert_id > _next_id ? concert_id + 1 : ++_next_id;
}

json Concert::to_json() const
{
    return json{
        {"concert_id", concert_id},
        {"artists", artists},
        {"venue", venue},
        {"city", city},
        {"date", date},
        {"cost", cost}
    };
}

bool Concert::operator==(const Concert& other) const
{
    return concert_id == other.get_concert_id();
}

int32_t Concert::get_concert_id() const { return concert_id; }
const std::unordered_map<int32_t, ArtistRoles>& Concert::get_artists() const { return artists; }
std::string Concert::get_venue() const { return venue; }
std::string Concert::get_city() const { return city; }
std::string Concert::get_date() const { return date; }
int32_t Concert::get_cost() const { return cost; }

void Concert::set_venue(std::string input) { venue = input; }
void Concert::set_city(std::string input) { city = input; }
void Concert::set_date(std::string input) { date = input; }
void Concert::set_cost(int32_t input) { cost = input; }

void Concert::add_artist(int32_t artist_id, ArtistRoles role)
{
    artists.insert_or_assign(artist_id, role);
}

void Concert::edit_artist(int32_t old_id, int32_t new_id, ArtistRoles role)
{
    auto it = artists.find(old_id);
    if (it == artists.end())
    {
        throw std::out_of_range("Artist ID not recognised");
    }

    artists.erase(it);
    artists.insert_or_assign(new_id, role);
}

void Concert::delete_artist(int32_t artist_id)
{
    if(artists.erase(artist_id) ==0)
    {
        throw std::out_of_range("Artist ID not recognised");
    }
}

void Concert::reset()
{
    _next_id = 1;
}
