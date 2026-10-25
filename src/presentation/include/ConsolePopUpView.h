#ifndef CONSOLE_POPUP_VIEW_H
#define CONSOLE_POPUP_VIEW_H

#include "PopUpView.h"

class ConsolePopUpView : public PopUpView
{
  public:
    void show_error_message(const std::string &message) override;
    void show_input_request(const std::string &message) override;
    void show_menu_option(const std::string &message) override;
};

#endif
