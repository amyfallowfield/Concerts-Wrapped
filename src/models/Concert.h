#ifndef CONCERT_H
#define CONCERT_H

#include <string>
#include <cstdint>

class Concert
{
private:
    std::string artist;
    std::string venue;
    std::string city;
    std::string date;
    int32_t cost;

public:
    Concert(std::string artist, std::string venue, std::string city, std::string date, int32_t cost);

    void print() const;
};

#endif