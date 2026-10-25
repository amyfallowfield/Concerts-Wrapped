#ifndef MOCK_MENU_VIEW_H
#define MOCK_MENU_VIEW_H

#include <gmock/gmock.h>

#include "MenuView.h"

class MenuViewMock : public MenuView
{
public:
    MOCK_METHOD(void, show_welcome, (), (override));
    MOCK_METHOD(void, show_main_menu, (), (override));
    MOCK_METHOD(void, show_edit_menu, (), (override));
    MOCK_METHOD(void, show_edit_artist_options, (), (override));
};

#endif
