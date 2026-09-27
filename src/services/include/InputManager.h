#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <cstdint>
#include <iostream>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "Logger.h"
#include "ValidationManager.h"

enum class Actions
{
    Add = 1,
    Edit,
    Delete,
    None
};

struct ArtistUpdateRequest
{
    bool success;
    Actions action = Actions::None;
    int artist_id = -1;
};

struct InputManager
{
public:
    int select_attribute();

    template <typename T>
    T get_attribute_input(
        const std::string& prompt,
        std::function<T(const std::string&)> input_method,
        std::function<T(T&)> transformation_method,
        std::function<ValidationResult<T>(T&)> validation_method)
    {
        while (true)
        {
            T input = input_method(prompt);

            ValidationResult<T> result = validation_method(input);

            if (result.is_valid) { return transformation_method(result.value); }

            std::cout << result.error_message;
            Logger::Warn("ConcertRepository", "get_attribute_input", result.error_message);
        }
    }

    ArtistUpdateRequest get_artist_update_data(const std::vector<std::pair<int32_t, std::string>>& artist_id_name_pair);

    std::string get_string_input(const std::string& prompt);
    int32_t get_numerical_input(const std::string& prompt);
    double get_decimal_input(const std::string& prompt);
    bool get_bool_input(const std::string& prompt);
    ArtistRoles get_role_input(const std::string& prompt);

private:
    int _get_bounded_numerical_int(int lower, int upper);
};

#endif
