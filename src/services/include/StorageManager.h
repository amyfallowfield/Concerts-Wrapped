#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <filesystem>
#include <fstream>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "Logger.h"
#include "Performance.h"

template<typename T>
struct FilePaths;

template<>
struct FilePaths<Artist>
{
    static inline const std::filesystem::path path = "artist_data.json";
};

template<>
struct FilePaths<Concert>
{
    static inline const std::filesystem::path path = "concert_data.json";
};

template<>
struct FilePaths<Performance>
{
    static inline const std::filesystem::path path = "performance_data.json";
};

class StorageManager
{
public:
    std::string directory;
    StorageManager(std::string directory)
    : directory(directory)
    {};

    template<typename T>
    bool save(const std::vector<T>& items)
    {
        json json_array = json::array();

        for (const T& item : items)
        {
            json_array.push_back(item.to_json());
        }

        std::filesystem::path path = _get_file_path<T>();
        std::filesystem::create_directories(path.parent_path());
        std::ofstream file(path);

        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Failed to open {}", path.string()));
            return false;
        }

        file << json_array.dump(4);

        Logger::Info("StorageManager", "save", path.string() + " saved successfully");

        return true;
    }

    template<typename T>
    std::vector<T> load()
    {
        std::vector<T> items;
        json json_array = json::array();

        std::filesystem::path path = _get_file_path<T>();
        std::ifstream file(path);

        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Failed to load {}", path.string()));
            return {};
        }

        if (file.peek() == std::ifstream::traits_type::eof()) { return items; }

        file >> json_array;
        for (const auto& item : json_array)
        {
            items.push_back(T{item});
        }

        Logger::Info("StorageManager", "load", std::to_string(items.size()) + " items from " + path.string() + " loaded successfully");

        return items;
    }

private:
    template<typename T>
    std::filesystem::path _get_file_path() const
    {
        return std::filesystem::path(directory) / FilePaths<T>::path;
    }
};

#endif
