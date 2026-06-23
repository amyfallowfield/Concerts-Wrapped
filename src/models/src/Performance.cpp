#include <nlohmann/json.hpp>

#include "Performance.h"

using json = nlohmann::json;

Performance::Performance(int32_t show_id, std::string artist, std::string role)
    : show_id(show_id),
      artist(artist),
      role(role)
{}

Performance::Performance(const json& data)
    : show_id(data.at("show_id")),
      artist(data.at("artist")),
      role(data.at("role"))
{}

json Performance::to_json() const
{
    return json{
        {"show_id", show_id},
        {"artist", artist},
        {"role", role},
    };
}

int32_t Performance::get_show_id() const { return show_id; };
std::string Performance::get_artist() const { return artist; };
std::string Performance::get_role() const { return role; };
