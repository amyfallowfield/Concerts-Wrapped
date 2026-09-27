#ifndef CONCERT_H
#define CONCERT_H

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

using json = nlohmann::json;

enum class ArtistRoles
{
    Headline,
    Support,
    Guest,
    None
};

std::string artist_role_to_string(ArtistRoles role);

class Concert
{
public:
    Concert(const std::unordered_map<int32_t, ArtistRoles>& artists_map, std::string venue, std::string city, std::string date, int32_t cost);
    Concert(const json& data);

    void print(const std::vector<std::pair<std::string, ArtistRoles>>& artist_name_role_pair) const;
    json to_json() const;
    bool operator==(const Concert& other) const;

    int32_t get_concert_id() const;
    const std::unordered_map<int32_t, ArtistRoles>& get_artists() const;
    std::string get_venue() const;
    std::string get_city() const;
    std::string get_date() const;
    int32_t get_cost() const;

    void set_venue(std::string input);
    void set_city(std::string input);
    void set_date(std::string input);
    void set_cost(int32_t input);

    void add_artist(int32_t artist_id, ArtistRoles role);
    void edit_artist(int32_t old_id, int32_t new_id, ArtistRoles role);
    void delete_artist(int32_t artist_id);

    static void reset();

private:
    static int32_t _next_id;

    int32_t concert_id;
    std::unordered_map<int32_t, ArtistRoles> artists;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;
};

#endif