#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "ComponentManager.h"
#include "ConcertRepository.h"
#include "IStorageManager.h"
#include "StatsManager.h"
#include "ValidationManager.h"

enum class Screen
{
    Menu,
    AddConcert,
    ViewConcerts,
    DeleteConcert,
    EditConcert,
    ConcertStats,
    ArtistStats,
    Exit
};

class ScreenManager
{
  public:
    ScreenManager(ConcertRepository &repo, IStorageManager &storage, UIComponent &ui);

    void run();
    void process_current_screen();

  private:
    IStorageManager &storage;
    ConcertRepository &repo;
    UIComponent &ui;
    Screen current_screen;

    ValidationManager validator;

    void show_menu();
    bool select_artist(const std::vector<int32_t> &choices, int32_t &selected_id);

    std::string enum_to_string(Screen screen);

    template <typename InputT, typename T>
    InputResult<T> _get_validated_input(const std::string &prompt,
                                        const std::string &invalid_input_message,
                                        std::function<InputResult<InputT>()> get_input,
                                        std::function<ValidationResult<T>(InputT &)> validate);
};

#endif
