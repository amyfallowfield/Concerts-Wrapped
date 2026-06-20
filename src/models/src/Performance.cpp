#include <nlohmann/json.hpp>

#include "Performance.h"

using json = nlohmann::json;

Performance::Performance(int show_id, std::string artist, std::string role)
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
