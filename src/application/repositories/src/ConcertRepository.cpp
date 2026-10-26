#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "Logger.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

ConcertRepository::ConcertRepository(IStorageManager &storage)
{
    artists = storage.load_artists();
    concerts = storage.load_concerts();
    concert_artists = storage.load_concert_artists();

    for (const ConcertArtist &concert_artist : concert_artists)
    {
        if (_find_concert_by_id(concert_artist.get_concert_id()) == concerts.end())
        {
            throw std::runtime_error("Concert-artist row references an unknown concert");
        }

        if (_find_artist_by_id(concert_artist.get_artist_id()) == artists.end())
        {
            throw std::runtime_error("Concert-artist row references an unknown artist");
        }
    }
}

void ConcertRepository::remove_concert(int32_t concert_id)
{
    auto deleted_concert_it = _find_concert_by_id(concert_id);

    if (deleted_concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }

    int32_t deleted_concert_id = deleted_concert_it->get_concert_id();
    concerts.erase(deleted_concert_it);
    std::erase_if(concert_artists, [&](const ConcertArtist &concert_artist)
                  { return concert_artist.get_concert_id() == deleted_concert_id; });
    _remove_unused_artists();

    LOG_INFO("Concert deleted successfully");
}

void ConcertRepository::add_artist(int32_t concert_id, std::string artist_name, ArtistRoles role)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }

    auto artist_it = _find_artist_by_name(artist_name);
    int32_t artist_id;
    if (artist_it == artists.end())
    {
        Artist artist(artist_name);
        artists.push_back(artist);
        artist_id = artist.get_id();
    }
    else
    {
        artist_id = artist_it->get_id();
    }

    const auto concert_artist_it =
        std::find_if(concert_artists.begin(), concert_artists.end(),
                     [&](const ConcertArtist &concert_artist)
                     {
                         return concert_artist.get_concert_id() == concert_id &&
                                concert_artist.get_artist_id() == artist_id;
                     });
    if (concert_artist_it != concert_artists.end())
    {
        LOG_ERROR("Artist is already associated with this concert");
        return;
    }

    concert_artists.emplace_back(concert_id, artist_id, role);
    LOG_INFO("Artist added successfully");
}

void ConcertRepository::edit_artist(int32_t concert_id, int32_t artist_id,
                                    int32_t replacement_artist_id, ArtistRoles role)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }

    auto concert_artist_it =
        std::find_if(concert_artists.begin(), concert_artists.end(),
                     [&](const ConcertArtist &concert_artist)
                     {
                         return concert_artist.get_concert_id() == concert_id &&
                                concert_artist.get_artist_id() == artist_id;
                     });
    if (concert_artist_it == concert_artists.end())
    {
        LOG_ERROR("Artist is not associated with this concert");
        return;
    }
    if (_find_artist_by_id(replacement_artist_id) == artists.end())
    {
        LOG_ERROR("Replacement artist ID not found");
        return;
    }
    if (artist_id != replacement_artist_id &&
        std::find_if(concert_artists.begin(), concert_artists.end(),
                     [&](const ConcertArtist &row) {
                         return row.get_concert_id() == concert_id &&
                                row.get_artist_id() == replacement_artist_id;
                     }) != concert_artists.end())
    {
        LOG_ERROR("Replacement artist is already associated with this concert");
        return;
    }

    concert_artist_it->set_artist_id(replacement_artist_id);
    concert_artist_it->set_role(role);
    _remove_unused_artists();
    LOG_INFO("Artist edited successfully");
}

void ConcertRepository::delete_artist(int32_t concert_id, int32_t artist_id)
{
    auto concert_artists_it =
        std::find_if(concert_artists.begin(), concert_artists.end(),
                     [&](const ConcertArtist &concert_artist)
                     {
                         return concert_artist.get_concert_id() == concert_id &&
                                concert_artist.get_artist_id() == artist_id;
                     });
    if (concert_artists_it == concert_artists.end())
    {
        LOG_ERROR("Concert or artist pair not found");
        return;
    }

    concert_artists.erase(concert_artists_it);
    _remove_unused_artists();
    LOG_INFO("Artist deleted successfully");
}

void ConcertRepository::update_venue(int32_t concert_id, std::string venue)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }
    concert_it->set_venue(venue);

    LOG_INFO("Concert venue editted successfully");
}

void ConcertRepository::update_city(int32_t concert_id, std::string city)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }
    concert_it->set_city(city);

    LOG_INFO("Concert city editted successfully");
}

void ConcertRepository::update_date(int32_t concert_id, std::string date)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }
    concert_it->set_date(date);

    LOG_INFO("Concert date editted successfully");
}

void ConcertRepository::update_cost(int32_t concert_id, double cost)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }
    concert_it->set_cost(cost * 100);

    LOG_INFO("Concert cost editted successfully");
}

const std::vector<Artist> &ConcertRepository::get_artists() const
{
    return artists;
}
const std::vector<Concert> &ConcertRepository::get_concerts() const
{
    return concerts;
}
const std::vector<ConcertArtist> &ConcertRepository::get_concert_artists() const
{
    return concert_artists;
}

void ConcertRepository::add_concert(std::map<std::string, ArtistRoles> unprocessed_artists,
                                    std::string venue, std::string city, std::string date,
                                    double cost)
{
    int32_t cost_as_int = static_cast<int32_t>(cost * 100);
    Concert new_concert = {venue, city, date, cost_as_int};

    for (const auto &[name, role] : unprocessed_artists)
    {
        auto artist_it = _find_artist_by_name(name);

        int32_t id;
        if (artist_it == artists.end())
        {
            Artist artist = Artist(name);
            artists.push_back(artist);
            id = artist.get_id();
        }
        else
        {
            id = artist_it->get_id();
        }

        concert_artists.emplace_back(new_concert.get_concert_id(), id, role);
    }

    concerts.push_back(new_concert);

    LOG_INFO("Concert created successfully");
}

void ConcertRepository::_remove_unused_artists()
{
    std::erase_if(artists,
                  [&](const Artist &artist)
                  {
                      bool artist_still_used =
                          std::any_of(concert_artists.begin(), concert_artists.end(),
                                      [&](const ConcertArtist &concert_artist) {
                                          return concert_artist.get_artist_id() == artist.get_id();
                                      });
                      return !artist_still_used;
                  });
}

std::vector<Concert>::iterator ConcertRepository::_find_concert_by_id(int32_t id)
{
    auto it = std::find_if(concerts.begin(), concerts.end(),
                           [&](const Concert &concert) { return concert.get_concert_id() == id; });

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_id(int32_t id)
{
    auto it = std::find_if(artists.begin(), artists.end(),
                           [&](const Artist &artist) { return artist.get_id() == id; });

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_name(const std::string &name)
{
    auto it = std::find_if(artists.begin(), artists.end(),
                           [&](const Artist &artist) { return artist.get_name() == name; });

    return it;
}
