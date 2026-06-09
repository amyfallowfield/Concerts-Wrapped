#ifndef ARTIST_H
#define ARTIST_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct Artist
{
    std::string name;
    std::string first_seen;
    std::string last_seen;
    int count;
    int total_cost;

    Artist(std::string name, std::string date, int cost);
    Artist(const json& data);

    json to_json() const;

    void update(std::string date, int cost);
};

#endif