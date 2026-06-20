#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "ValidationManager.h"

class ConcertRepository
{
public:
    ConcertRepository();

    void add();
    void print();

    std::vector<Concert> get_concerts();
    std::vector<Artist> get_artists();

private:
    std::vector<Artist> artists;
    std::vector<Concert> concerts;
    ValidationManager validator = ValidationManager();

    Concert create_concert();
    void update_artists(const Concert& new_concert);

    std::string _get_string_input(std::string prompt);
    int32_t _get_monetary_input(std::string prompt);

    template <typename T>
    T get_input(
        const std::string& prompt,
        std::function<T(const std::string&)> input_method,
        std::function<T(T&)> transformation_method,
        std::function<ValidationResult<T>(T&)> validation_method
    );
};

#endif
