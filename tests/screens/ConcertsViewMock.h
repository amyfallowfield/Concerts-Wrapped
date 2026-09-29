#ifndef MOCK_CONCERTS_VIEW_H
#define MOCK_CONCERTS_VIEW_H

#include <gmock/gmock.h>
#include "ConcertsView.h"

class ConcertsViewMock : public ConcertsView
{
public:
    MOCK_METHOD(void, show, (const std::vector<Concert>&, const std::vector<Artist>&), (override));
};

#endif
