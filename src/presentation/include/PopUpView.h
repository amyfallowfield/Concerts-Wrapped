#ifndef POPUP_VIEW_H
#define POPUP_VIEW_H

#include <string>

class PopUpView
{
public:
    virtual void show_error_message(const std::string& message) = 0;
};

#endif
