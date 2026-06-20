#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <fstream>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "Logger.h"

template<typename T>
struct FilePaths;

template<>
struct FilePaths<Concert>
{
    static constexpr const char* path = "concert_data.json";
};

template<>
struct FilePaths<Artist>
{
    static constexpr const char* path = "artist_data.json";
};

struct StorageManager
{
    template<typename T>
    bool save(const std::vector<T>& items)
    {
        json json_array = json::array();

        for (const T& item : items)
        {
            json_array.push_back(item.to_json());
        }

        std::ofstream file(FilePaths<T>::path);

        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Failed to open {}", FilePaths<T>::path));
            return false;
        }

        file << json_array.dump(4);

        Logger::Info("StorageManager", "save", std::string(FilePaths<T>::path) + " saved successfully");

        return true;
    }

    template<typename T>
    std::vector<T> load()
    {
        std::vector<T> items;
        json json_array = json::array();

        std::ifstream file(FilePaths<T>::path);

        if (!file.is_open()) { return {}; }

        if (file.peek() == std::ifstream::traits_type::eof()) { return items; }

        file >> json_array;
        for (const auto& item : json_array)
        {
            items.push_back(T{item});
        }

        Logger::Info("StorageManager", "load", std::to_string(items.size()) + " items from " + std::string(FilePaths<T>::path) + " loaded successfully");

        return items;
    }
};

#endif
