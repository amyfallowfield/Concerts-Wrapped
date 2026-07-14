#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <functional>

#include "Logger.h"
#include "ValidationManager.h"

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

    std::string get_string_input(std::string prompt);
    int32_t get_numerical_input(std::string prompt);
    double get_decimal_input(std::string prompt);
};

#endif
