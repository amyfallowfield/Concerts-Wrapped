#ifndef CONCERTS_VIEW_H
#define CONCERTS_VIEW_H

#include "Artist.h"
#include "Concert.h"
#include <vector>

class ConcertsView
{
  public:
    virtual void show(const std::vector<Concert> &concerts, const std::vector<Artist> &artists) = 0;
};

#endif
