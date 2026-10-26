#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <map>
#include <string>
#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertArtist.h"
#include "IStorageManager.h"

class ConcertRepository
{
  public:
    ConcertRepository(IStorageManager &storage);

    virtual void add_concert(std::map<std::string, ArtistRoles> artists, std::string venue,
                             std::string city, std::string date, double cost);
    virtual void remove_concert(int32_t concert_id);
    virtual void add_artist(int32_t concert_id, std::string artist_name, ArtistRoles role);
    virtual void edit_artist(int32_t concert_id, int32_t artist_id, int32_t replacement_artist_id,
                             ArtistRoles role);
    virtual void delete_artist(int32_t concert_id, int32_t artist_id);
    virtual void update_venue(int32_t concert_id, std::string venue);
    virtual void update_city(int32_t concert_id, std::string city);
    virtual void update_date(int32_t concert_id, std::string date);
    virtual void update_cost(int32_t concert_id, double cost);

    const std::vector<Concert> &get_concerts() const;
    const std::vector<Artist> &get_artists() const;
    const std::vector<ConcertArtist> &get_concert_artists() const;

  private:
    std::vector<Artist> artists;
    std::vector<Concert> concerts;
    std::vector<ConcertArtist> concert_artists;

    void _remove_unused_artists();

    std::vector<Concert>::iterator _find_concert_by_id(int32_t id);
    std::vector<Artist>::iterator _find_artist_by_id(int32_t id);
    std::vector<Artist>::iterator _find_artist_by_name(const std::string &name);
    std::pair<int32_t, ArtistRoles> _get_new_artist_id_role();
};

#endif
