#ifndef CONCERT_STATS_MANAGER_MOCK_H
#define CONCERT_STATS_MANAGER_MOCK_H

#include <gmock/gmock.h>

#include "ConcertStatsManager.h"

class ConcertStatsManagerMock : public ConcertStatsManager
{
public:
    MOCK_METHOD(void, print_stats, (const std::vector<Concert>& concerts), (override));
};

#endif
