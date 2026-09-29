#ifndef MOCK_STATS_VIEW_H
#define MOCK_STATS_VIEW_H

#include <gmock/gmock.h>

#include "StatsView.h"

class StatsViewMock : public StatsView
{
public:
    MOCK_METHOD(void, show_concert_stats, (const ConcertStats&), (override));
};

#endif
