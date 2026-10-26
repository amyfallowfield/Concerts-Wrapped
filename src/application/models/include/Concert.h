#ifndef CONCERT_H
#define CONCERT_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Concert
{
  public:
    Concert(std::string venue, std::string city, std::string date, int32_t cost);
    Concert(const json &data);

    json to_json() const;
    bool operator==(const Concert &other) const;

    int32_t get_concert_id() const;
    std::string get_venue() const;
    std::string get_city() const;
    std::string get_date() const;
    int32_t get_cost() const;

    void set_venue(std::string input);
    void set_city(std::string input);
    void set_date(std::string input);
    void set_cost(int32_t input);

    static void reset();

  private:
    static int32_t _next_id;

    int32_t concert_id;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;
};

#endif