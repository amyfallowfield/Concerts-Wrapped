#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "ConcertsView.h"
#include "StatsView.h"
#include "MenuView.h"
#include "PopUpView.h"

struct UIComponent
{
    ConcertsView& concerts_view;
    StatsView& stats_view;
    MenuView& menu_view;
    PopUpView& pop_up_view;
};

#endif
