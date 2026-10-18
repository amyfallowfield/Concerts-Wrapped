#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "Logger.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

ConcertRepository::ConcertRepository(StorageManager& storage)
{
    artists = storage.load<Artist>();
    concerts = storage.load<Concert>();
}

void ConcertRepository::remove_concert(int32_t concert_id)
{
    auto deleted_concert_it = _find_concert_by_id(concert_id);

    if (deleted_concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
        return;
    }

    Concert deleted_concert = *deleted_concert_it;
    concerts.erase(deleted_concert_it);
    _refresh_artists(deleted_concert);

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

    if (concert_it->get_artists().find(artist_id) != concert_it->get_artists().end())
    {
        LOG_ERROR("Artist is already associated with this concert");
        return;
    }

    concert_it->add_artist(artist_id, role);
    LOG_INFO("Artist added successfully");
}

void ConcertRepository::edit_artist(int32_t concert_id, int32_t artist_id, int32_t replacement_artist_id, ArtistRoles role)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end())
    {
        LOG_ERROR("Invalid concert ID selected");
    }

    const auto& concert_artists = concert_it->get_artists();
    if (concert_artists.find(artist_id) == concert_artists.end())
    {
        LOG_ERROR("Artist is not associated with this concert");
    }
    if (_find_artist_by_id(replacement_artist_id) == artists.end())
    {
        LOG_ERROR("Replacement artist ID not found");
    }
    if (artist_id != replacement_artist_id &&
        concert_artists.find(replacement_artist_id) != concert_artists.end())
    {
        LOG_ERROR("Replacement artist is already associated with this concert");
    }

    Concert old_concert = *concert_it;
    concert_it->edit_artist(artist_id, replacement_artist_id, role);
    _refresh_artists(old_concert);
    LOG_INFO("Artist edited successfully");
}

void ConcertRepository::delete_artist(int32_t concert_id, int32_t artist_id)
{
    auto concert_it = _find_concert_by_id(concert_id);
    if (concert_it == concerts.end() ||
        concert_it->get_artists().find(artist_id) == concert_it->get_artists().end())
    {
        LOG_ERROR("Concert or artist association not found");
    }

    Concert old_concert = *concert_it;
    concert_it->delete_artist(artist_id);
    _refresh_artists(old_concert);
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

const std::vector<Artist>&  ConcertRepository::get_artists() const { return artists; }
const std::vector<Concert>&  ConcertRepository::get_concerts() const { return concerts; }

void ConcertRepository::add_concert(
    std::map<std::string, ArtistRoles> unprocessed_artists,
    std::string venue,
    std::string city,
    std::string date,
    double cost)
{
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map;
    for (const auto& [name, role] : unprocessed_artists)
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

        artist_id_role_map.insert_or_assign(id, role);
    }

    int32_t cost_as_int = static_cast<int32_t>(cost * 100);

    Concert new_concert = {artist_id_role_map, venue, city, date, cost_as_int};
    concerts.push_back(new_concert);
    _refresh_artists(new_concert);

    LOG_INFO("Concert created successfully");
}

void ConcertRepository::_refresh_artists(const Concert& concert)
{
    for (const auto& artist : concert.get_artists())
    {
        int32_t artist_id = artist.first;
        auto artist_it = _find_artist_by_id(artist_id);

        if (artist_it == artists.end())
        {
            LOG_ERROR("Concert contains invalid artist ID");
            continue;
        }

        bool artist_still_used = std::any_of(
            concerts.begin(), concerts.end(),
            [&](const Concert& concert)
            {
                const auto& concert_artists = concert.get_artists();
                return concert_artists.find(artist_id) != concert_artists.end();
            }
        );

        if (!artist_still_used)
        {
            artists.erase(artist_it);
        }

        LOG_INFO("Artists successfully updated");
    }
}

std::vector<Concert>::iterator ConcertRepository::_find_concert_by_id(int32_t id)
{
    auto it = std::find_if(
        concerts.begin(), concerts.end(),
        [&](const Concert& concert)
        {
            return concert.get_concert_id() == id;
        }
    );

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_id(int32_t id)
{
    auto it = std::find_if(
        artists.begin(), artists.end(),
        [&](const Artist& artist)
        {
            return artist.get_id() == id;
        }
    );

    return it;
}

std::vector<Artist>::iterator ConcertRepository::_find_artist_by_name(const std::string& name)
{
    auto it = std::find_if(
        artists.begin(), artists.end(),
        [&](const Artist& artist)
        {
            return artist.get_name() == name;
        }
    );

    return it;
}
