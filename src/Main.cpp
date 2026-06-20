#include <iostream>
#include <limits>
#include <vector>

#include "ScreenManager.h"

int main()
{
    try
    {
        ScreenManager screen_manager = ScreenManager();
        screen_manager.run();
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatal error: " << e.what() << "\n";
    }
}
