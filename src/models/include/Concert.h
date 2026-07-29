#ifndef CONCERT_H
#define CONCERT_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class Concert
{
public:
    Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost, std::vector<std::string> supports);
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
    const std::vector<std::string>& get_supports() const;

    void set_artist(std::string input);
    void set_venue(std::string input);
    void set_city(std::string input);
    void set_date(std::string input);
    void set_cost(int32_t input);
    void set_supports(std::vector<std::string> input);

    static void reset();

private:
    static int32_t _next_id;

    int32_t id;
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;
    std::vector<std::string> supports;
};

#endif