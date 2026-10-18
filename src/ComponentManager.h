#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "ConcertsView.h"
#include "Input.h"
#include "MenuView.h"
#include "PopUpView.h"
#include "StatsView.h"

struct UIComponent
{
    ConcertsView& concerts_view;
    Input& input;
    MenuView& menu_view;
    PopUpView& pop_up_view;
    StatsView& stats_view;
};

#endif
