#include <vector>

#include "models/Concert.h"

struct ConcertStatsManager
{
    void print_stats(const std::vector<Concert>& concerts);
    int32_t total_shows(const std::vector<Concert>& concerts);
    int32_t total_cost(const std::vector<Concert>& concerts);
    int32_t average_cost(const std::vector<Concert>& concerts);
};