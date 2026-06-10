#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ValidationManager.h"

class ConcertRepository
{
private:
    std::vector<Artist> artists;
    std::vector<Concert> concerts;
    ValidationManager validator = ValidationManager();

    Concert create_concert();
    void update_artists(const Concert& new_concert);

public:
    ConcertRepository();

    void add();
    void print();
    std::vector<Concert> get_concerts();
    std::vector<Artist> get_artists();
};

#endif
