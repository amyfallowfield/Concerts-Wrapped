#include <ctime>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Artist.h"
#include "Utilities.h"

using json = nlohmann::json;

Artist::Artist(std::string artist, const std::vector<Concert>& concerts)
    : name(artist),
      first_seen(concerts.front().get_date()),
      last_seen(concerts.front().get_date()),
      count(concerts.size()),
      total_cost(0)
{
    for (const Concert& concert : concerts)
    {
        std::tm first_seen_tm_date = Utilities::parse_date(first_seen);
        time_t first_seen_date = std::mktime(&first_seen_tm_date);

        std::tm last_seen_tm_date = Utilities::parse_date(last_seen);
        time_t last_seen_date = std::mktime(&last_seen_tm_date);

        std::tm concert_tm_date = Utilities::parse_date(concert.get_date());
        time_t concert_date = std::mktime(&concert_tm_date);

        first_seen = std::difftime(concert_date, first_seen_date) < 0 ? concert.get_date() : first_seen;
        last_seen = std::difftime(concert_date, last_seen_date) > 0 ? concert.get_date() : last_seen;
        total_cost += concert.get_cost();
    }
}

Artist::Artist(const json& data)
    : name(data.at("name")),
      first_seen(data.at("first_seen")),
      last_seen(data.at("last_seen")),
      count(data.at("count")),
      total_cost(data.at("total_cost"))
{}

json Artist::to_json() const
{
    return json{
        {"name", name},
        {"first_seen", first_seen},
        {"last_seen", last_seen},
        {"count", count},
        {"total_cost", total_cost}
    };
}

std::string Artist::get_name() const { return name; };
std::string Artist::get_first_seen() const { return first_seen; };
std::string Artist::get_last_seen() const { return last_seen; };
int Artist::get_count() const { return count; };
int Artist::get_total_cost() const { return total_cost; };
