#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "Artist.h"
#include "Concert.h"
#include "Performance.h"
#include "StorageManager.h"
#include "ValidationManager.h"

class ConcertRepository
{
public:
    ConcertRepository();

    void add();
    void remove();
    void edit();
    void print();

    std::vector<Concert> get_concerts();
    std::vector<Artist> get_artists();
    std::vector<Performance> get_performances();

private:
    StorageManager storage = StorageManager();
    std::vector<Artist> artists;
    std::vector<Concert> concerts;
    std::vector<Performance> performances;
    ValidationManager validator = ValidationManager();

    Concert create_concert();
    void refresh_artist(std::string artist);
    void update_performances(const Concert& new_concert);

    Concert& _get_concert_from_id(int id);

    std::string _get_string_input(std::string prompt);
    int32_t _get_numerical_input(std::string prompt);
    double _get_decimal_input(std::string prompt);

    template <typename T>
    T get_input(
        const std::string& prompt,
        std::function<T(const std::string&)> input_method,
        std::function<T(T&)> transformation_method,
        std::function<ValidationResult<T>(T&)> validation_method
    );
    int get_concert_id();
};

#endif
