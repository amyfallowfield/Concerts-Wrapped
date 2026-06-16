#include <ctime>
#include <regex>
#include <vector>

#include "Concert.h"
#include "Logger.h"
#include "ValidationManager.h"

ValidationResult<std::string> ValidationManager::validate_artist(std::string& input)
{
    clean_string(input);
    return validate_string(input, "Artist");
}

ValidationResult<std::string> ValidationManager::validate_venue(std::string& input)
{
    clean_string(input);
    return validate_string(input, "Venue");
}

ValidationResult<std::string> ValidationManager::validate_city(std::string& input)
{
    clean_string(input);
    return validate_string(input, "City");
}

ValidationResult<std::string> ValidationManager::validate_date(std::string& input)
{
    clean_string(input);

    ValidationResult<std::string> validated_string = validate_string(input, "Date");
    if (!validated_string.is_valid) { return validated_string; }

    std::regex pattern(R"(^\d{2}-\d{2}-\d{4}$)");

    if (!std::regex_match(input, pattern)) { return {false, input, "Date must use format DD-MM-YYYY"}; }

    int day = std::stoi(input.substr(0, 2));
    int month = std::stoi(input.substr(3, 2));
    int year = std::stoi(input.substr(6, 4));

    std::tm tm_date = {};
    tm_date.tm_mday = day;
    tm_date.tm_mon  = month - 1;
    tm_date.tm_year = year - 1900;

    time_t event_date = std::mktime(&tm_date);
    time_t now = std::time(nullptr);

    bool invalid_date = (
        day != tm_date.tm_mday ||
        month - 1 != tm_date.tm_mon ||
        year - 1900 != tm_date.tm_year
    );

    if (invalid_date) { return {false, input, "Date does not exist"}; }
    if (event_date > now) { return {false, input, "Date cannot be in the future"}; }

    return {true, input, {}};
}

ValidationResult<int32_t> ValidationManager::validate_cost(int32_t& input)
{
    if (input < 0) { return {false, input, "Cost cannot be negative"}; }
    
    return {true, input, {}};
}

ValidationResult<std::string> ValidationManager::validate_string(std::string& input, const std::string& attribute)
{
    if (input.find_first_not_of(WHITESPACE) == std::string::npos)
    {
        return {false, input, attribute + " must contain non-whitespace characters"};
    }

    return {true, input, {}};
}

void ValidationManager::clean_string(std::string& input)
{
    std::string original = input;

    if (input.find_first_not_of(WHITESPACE) == std::string::npos) { return; }

    input.erase(0, input.find_first_not_of(WHITESPACE));
    input.erase(input.find_last_not_of(WHITESPACE) + 1);

    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });

    if (original != input)
    {
        Logger::Warn("ValidationManager", "clean_string", "Input has been cleaned from " + original + " to " + input);
    }
}
