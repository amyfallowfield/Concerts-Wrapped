#ifndef CONSOLE_CONCERTS_VIEW_H
#define CONSOLE_CONCERTS_VIEW_H

#include "ConcertsView.h"

class ConsoleConcertsView : public ConcertsView
{
public:
    void show(const std::vector<Concert>& concerts, const std::vector<Artist>& artists) override;
};

#endif
