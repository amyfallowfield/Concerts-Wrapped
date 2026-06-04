#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "models/Concert.h"

class ConcertRepository
{
private:
    std::vector<Concert> concerts;

public:
    void add();
    void print();
    std::vector<Concert> get_all();
};

#endif