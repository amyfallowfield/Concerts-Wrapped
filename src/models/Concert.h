#ifndef CONCERT_H
#define CONCERT_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Concert
{
private:
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;

public:
    Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost);
    Concert(const json& data);

    void print() const;
    json to_json() const;
};

#endif