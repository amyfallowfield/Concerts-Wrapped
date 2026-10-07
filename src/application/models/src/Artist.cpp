#include <nlohmann/json.hpp>
#include <string>

#include "Artist.h"

using json = nlohmann::json;

int32_t Artist::_next_id = 1;

Artist::Artist(const std::string& name)
    : id(_next_id),
      name(name)
{
    _next_id = id > _next_id ? ++id : ++_next_id;
}

Artist::Artist(const json& data)
    : id(data.at("id")),
      name(data.at("name"))
{
    _next_id = id > _next_id ? id + 1 : ++_next_id;
}

json Artist::to_json() const
{
    return json{
        {"id", id},
        {"name", name}
    };
}

int32_t Artist::get_id() const { return id; }
std::string Artist::get_name() const { return name; }

void Artist::set_name(std::string new_name) { name = new_name; }

void Artist::reset()
{
    _next_id = 1;
}
