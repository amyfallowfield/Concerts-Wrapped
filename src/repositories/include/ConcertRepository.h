#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "InputManager.h"
#include "Performance.h"
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
    std::vector<Performance> get_performances();

private:
    StorageManager storage = StorageManager("data");
    std::vector<Artist> artists;
    std::vector<Concert> concerts;
    std::vector<Performance> performances;

    ValidationManager validator = ValidationManager();
    InputManager input_manager = InputManager();

    Concert create_concert();
    void _refresh_artists(const Concert& concert);
    void update_performances(const Concert& new_concert);

    Concert& _get_concert_from_id(int id);

    int get_concert_id();
};

#endif
