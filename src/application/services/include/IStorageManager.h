#ifndef ISTORAGE_MANAGER_H
#define ISTORAGE_MANAGER_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ConcertArtist.h"

class IStorageManager
{
  public:
    virtual bool save_artists(const std::vector<Artist> &items) = 0;
    virtual bool save_concert_artists(const std::vector<ConcertArtist> &items) = 0;
    virtual bool save_concerts(const std::vector<Concert> &items) = 0;

    virtual std::vector<Artist> load_artists() = 0;
    virtual std::vector<ConcertArtist> load_concert_artists() = 0;
    virtual std::vector<Concert> load_concerts() = 0;
};

#endif
