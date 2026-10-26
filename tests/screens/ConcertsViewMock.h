#ifndef MOCK_CONCERTS_VIEW_H
#define MOCK_CONCERTS_VIEW_H

#include "ConcertsView.h"
#include <gmock/gmock.h>

class ConcertsViewMock : public ConcertsView
{
  public:
    MOCK_METHOD(void, show,
                (const std::vector<Concert> &, const std::vector<Artist> &,
                 const std::vector<ConcertArtist> &),
                (override));
};

#endif
