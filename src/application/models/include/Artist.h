#ifndef ARTIST_H
#define ARTIST_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Artist
{
public:
    Artist(const std::string& name);
    Artist(const json& data);

    json to_json() const;

    int get_id() const;
    std::string get_name() const;

    void set_name(std::string new_name);

    static void reset();

private:
    int32_t id;
    std::string name;

    static int32_t _next_id;
};

#endif
