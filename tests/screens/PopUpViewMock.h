#ifndef MOCK_POPUP_VIEW_H
#define MOCK_POPUP_VIEW_H

#include <gmock/gmock.h>

#include "PopUpView.h"

class PopUpViewMock : public PopUpView
{
  public:
    MOCK_METHOD(void, show_error_message, (const std::string &), (override));
    MOCK_METHOD(void, show_input_request, (const std::string &), (override));
    MOCK_METHOD(void, show_menu_option, (const std::string &), (override));
};

#endif
