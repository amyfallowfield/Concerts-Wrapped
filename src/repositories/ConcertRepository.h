#ifndef CONCERT_REPOSITORY_H
#define CONCERT_REPOSITORY_H

#include <vector>

#include "models/Concert.h"
#include "services/ValidationManager.h"

class ConcertRepository
{
private:
    std::vector<Concert> concerts;
    ValidationManager validator = ValidationManager();

public:
    void add();
    void print();
    std::vector<Concert> get_all();
};

#endif