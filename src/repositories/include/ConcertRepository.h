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

    std::string get_input(std::string prompt, std::function<ValidationResult<std::string>(std::string&)> validation_method);
    int32_t get_monetary_input(std::string prompt, std::function<ValidationResult<int32_t>(int32_t)> validation_method);

public:
    ConcertRepository();

    void add();
    void print();
    std::vector<Concert> get_concerts();
    std::vector<Artist> get_artists();
};

#endif
