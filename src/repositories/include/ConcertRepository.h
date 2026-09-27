#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "InputManager.h"
#include "StorageManager.h"
#include "ValidationManager.h"

class ConcertRepository
{
public:
    ConcertRepository(StorageManager& storage);

    virtual void add();
    virtual void remove();
    virtual void edit();
    virtual void print();

    std::vector<Concert> get_concerts();
    std::vector<Artist> get_artists();

private:
    StorageManager storage = StorageManager("data");
    std::vector<Artist> artists;
    std::vector<Concert> concerts;

    ValidationManager validator = ValidationManager();
    InputManager input_manager = InputManager();

    Concert create_concert();
    void _refresh_artists(const Concert& concert);

    std::vector<Concert>::iterator _find_concert_by_id(int32_t id);
    std::vector<Artist>::iterator _find_artist_by_id(int32_t id);
    std::vector<Artist>::iterator _find_artist_by_name(std::string name);

    int get_concert_id();
};

#endif
