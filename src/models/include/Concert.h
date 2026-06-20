#ifndef CONCERT_H
#define CONCERT_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Concert
{
public:
    Concert(int32_t id, std::string artist, std::string venue, std::string city, std::string date, int32_t cost);
    Concert(const json& data);

    void print() const;
    json to_json() const;
    bool operator==(const Concert& other) const;

    int32_t get_id() const;
    std::string get_artist() const;
    std::string get_venue() const;
    std::string get_city() const;
    std::string get_date() const;
    int32_t get_cost() const;

private:
    int32_t id;
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;
};

#endif