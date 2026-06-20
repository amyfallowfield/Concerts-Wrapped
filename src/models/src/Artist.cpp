#include <nlohmann/json.hpp>
#include <string>

#include "Artist.h"

using json = nlohmann::json;

Artist::Artist(std::string name, std::string date, int cost)
    : name(name),
      first_seen(date),
      last_seen(date),
      count(1),
      total_cost(cost)
{}

Artist::Artist(const json& data)
    : name(data.at("name")),
      first_seen(data.at("first_seen")),
      last_seen(data.at("last_seen")),
      count(data.at("count")),
      total_cost(data.at("total_cost"))
{}

void Artist::update(const Concert& concert)
{
    if (concert.get_date() > last_seen)
    {
        last_seen = concert.get_date();
    }
    count ++;
    total_cost += concert.get_cost();
}

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
