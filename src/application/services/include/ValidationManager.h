#ifndef VALIDATION_MANAGER_H
#define VALIDATION_MANAGER_H

#include <iostream>
#include <type_traits>
#include <vector>

#include "Concert.h"

template <typename T>
struct ValidationResult
{
    bool is_valid;
    T value;
    std::string error_message;
};

class ValidationManager
{
public:
    ValidationResult<std::string> validate_artist(std::string& input);
    ValidationResult<std::string> validate_venue(std::string& input);
    ValidationResult<std::string> validate_city(std::string& input);
    ValidationResult<std::string> validate_date(std::string& input);
    ValidationResult<double> validate_cost(double& input);
    ValidationResult<int> validate_id(int& input, const std::vector<Concert>& concerts);
    ValidationResult<ArtistRoles> validate_role(std::string& input);

    template <typename T>
    bool is_within_bounds(T value, T lower, T upper)
    {
        return value >= lower && value <= upper;
    }

private:
    static constexpr auto WHITESPACE = " \t\n\r";

    ValidationResult<std::string> validate_string(std::string& input, const std::string& attribute);
    void clean_string(std::string& input);
};

#endif
