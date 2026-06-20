#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <filesystem>
#include <fstream>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "Logger.h"

template<typename T>
struct FilePaths;

template<>
struct FilePaths<Artist>
{
    static inline const std::filesystem::path path =
    std::filesystem::path("data") / "artist_data.json";
};

template<>
struct FilePaths<Concert>
{
    static inline const std::filesystem::path path =
    std::filesystem::path("data") / "concert_data.json";
};

template<>
struct FilePaths<Performance>
{
    static inline const std::filesystem::path path =
    std::filesystem::path("data") / "performance_data.json";
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

        std::filesystem::create_directories(FilePaths<T>::path.parent_path());
        std::ofstream file(FilePaths<T>::path);

        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Failed to open {}", FilePaths<T>::path.string()));
            return false;
        }

        file << json_array.dump(4);

        Logger::Info("StorageManager", "save", FilePaths<T>::path.string() + " saved successfully");

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

        Logger::Info("StorageManager", "load", std::to_string(items.size()) + " items from " + FilePaths<T>::path.string() + " loaded successfully");

        return items;
    }
};

#endif
