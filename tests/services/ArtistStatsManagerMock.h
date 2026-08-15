#ifndef ARTIST_STATS_MANAGER_MOCK_H
#define ARTIST_STATS_MANAGER_MOCK_H

#include <gmock/gmock.h>

#include "ArtistStatsManager.h"

class ArtistStatsManagerMock : public ArtistStatsManager
{
public:
    MOCK_METHOD(void, print_stats, (const std::vector<Artist>& artists), (override));
};

#endif
