#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

#include "StorageManager.h"
#include "models/Concert.h"

using json = nlohmann::json;

bool StorageManager::save(const std::vector<Concert>& concerts)
{
    json json_array = json::array();

    for (const Concert& concert : concerts)
    {
        json_array.push_back(concert.to_json());
    }

    std::ofstream file("concert_data.json");

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open concert_data.json");
        return false;
    }

    file << json_array.dump(4);
    return true;
}

std::vector<Concert> StorageManager::load()
{
    std::vector<Concert> concerts;
    json json_array = json::array();

    std::ifstream file("concert_data.json");

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return concerts;
    }

    file >> json_array;
    for (const auto& concert : json_array)
    {
        concerts.push_back(Concert{concert});
    }

    return concerts;
}