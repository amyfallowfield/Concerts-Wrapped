#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertArtist.h"
#include "JSONStorageManager.h"
#include "Logger.h"

JSONStorageManager::JSONStorageManager(std::string directory) : directory(directory)
{
}

bool JSONStorageManager::save_artists(const std::vector<Artist> &items)
{
    return _save<Artist>(items);
}

bool JSONStorageManager::save_concert_artists(const std::vector<ConcertArtist> &items)
{
    return _save<ConcertArtist>(items);
}

bool JSONStorageManager::save_concerts(const std::vector<Concert> &items)
{
    return _save<Concert>(items);
}

std::vector<Artist> JSONStorageManager::load_artists()
{
    return _load<Artist>();
}

std::vector<ConcertArtist> JSONStorageManager::load_concert_artists()
{
    return _load<ConcertArtist>();
}

std::vector<Concert> JSONStorageManager::load_concerts()
{
    return _load<Concert>();
}

template <typename T> std::vector<T> JSONStorageManager::_load()
{
    std::vector<T> items;
    json json_array = json::array();

    std::filesystem::path path = _get_file_path<T>();
    if (!std::filesystem::exists(path))
    {
        return items;
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to load " + path.string());
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return items;
    }

    file >> json_array;
    for (const auto &item : json_array)
    {
        items.push_back(T{item});
    }

    Logger::Info("StorageManager", "load",
                 std::to_string(items.size()) + " items from " + path.string() +
                     " loaded successfully");
    return items;
}

template <typename T> bool JSONStorageManager::_save(const std::vector<T> &items)
{
    json json_array = json::array();

    for (const T &item : items)
    {
        json_array.push_back(item.to_json());
    }

    std::filesystem::path path = _get_file_path<T>();
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open " + path.string());
    }

    file << json_array.dump(4);

    Logger::Info("StorageManager", "save", path.string() + " saved successfully");

    return true;
}

template <typename T> std::filesystem::path JSONStorageManager::_get_file_path() const
{
    return std::filesystem::path(directory) / FilePaths<T>::path;
}
