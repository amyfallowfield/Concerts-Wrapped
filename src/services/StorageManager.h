#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>

#include "models/Concert.h"

struct StorageManager
{
    bool save(const std::vector<Concert>& concerts);
    std::vector<Concert> load();
};

#endif