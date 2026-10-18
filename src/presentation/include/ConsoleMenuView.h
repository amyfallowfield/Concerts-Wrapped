#ifndef CONSOLE_MENU_VIEW_H
#define CONSOLE_MENU_VIEW_H

#include "MenuView.h"

class ConsoleMenuView : public MenuView
{
    void show_welcome() override;
    void show_main_menu() override;
    void show_edit_menu() override;
    void show_edit_artist_options() override;
};

#endif
