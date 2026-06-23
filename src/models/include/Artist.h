#ifndef ARTIST_H
#define ARTIST_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

#include "Concert.h"

using json = nlohmann::json;

class Artist
{
public:
    Artist(const Concert& concert);
    Artist(const json& data);

    void update(const Concert& concert);

    json to_json() const;

    std::string get_name() const;
    std::string get_first_seen() const;
    std::string get_last_seen() const;
    int get_count() const;
    int get_total_cost() const;

private:
    std::string name;
    std::string first_seen;
    std::string last_seen;
    int count;
    int total_cost;
};

#endif
