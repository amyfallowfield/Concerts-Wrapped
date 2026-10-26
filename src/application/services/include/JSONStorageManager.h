#ifndef JSON_STORAGE_MANAGER_H
#define JSON_STORAGE_MANAGER_H

#include <filesystem>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "IStorageManager.h"
#include "Logger.h"

template <typename T> struct FilePaths;

template <> struct FilePaths<Artist>
{
    static inline const std::filesystem::path path = "artist_data.json";
};

template <> struct FilePaths<Concert>
{
    static inline const std::filesystem::path path = "concert_data.json";
};

template <> struct FilePaths<ConcertArtist>
{
    static inline const std::filesystem::path path = "concert_artists_data.json";
};

class JSONStorageManager : public IStorageManager
{
  public:
    std::string directory;
    JSONStorageManager(std::string directory);

    bool save_artists(const std::vector<Artist> &items) override;
    bool save_concert_artists(const std::vector<ConcertArtist> &items) override;
    bool save_concerts(const std::vector<Concert> &items) override;

    std::vector<Artist> load_artists() override;
    std::vector<ConcertArtist> load_concert_artists() override;
    std::vector<Concert> load_concerts() override;

  private:
    template <typename T> bool _save(const std::vector<T> &items);

    template <typename T> std::vector<T> _load();

    template <typename T> std::filesystem::path _get_file_path() const;
};

#endif
