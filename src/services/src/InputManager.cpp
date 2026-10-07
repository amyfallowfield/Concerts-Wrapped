#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "InputManager.h"
#include "Utilities.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

int InputManager::select_attribute()
{
    std::cout << "1. Artists\n";
    std::cout << "2. Venue\n";
    std::cout << "3. City\n";
    std::cout << "4. Date\n";
    std::cout << "5. Cost\n";
    std::cout << "Selection: ";

    int input = _get_bounded_numerical_int(1, 6);

    std::cout << '\n';

    return input;
}

ArtistUpdateRequest InputManager::get_artist_update_data(const std::vector<std::pair<int32_t, std::string>>& artist_id_name_pair)
{
    ArtistUpdateRequest output;

    std::cout << "1. Add Artist\n";
    std::cout << "2. Edit Artist\n";
    std::cout << "3. Delete Artist\n";
    std::cout << "Selection: ";

    int action_input = _get_bounded_numerical_int(1, 3);
    if (action_input == -1)
    {
        output.success = false;
        return output;
    }

    if (action_input == static_cast<int>(Actions::Edit) || action_input == static_cast<int>(Actions::Delete))
    {
        if(artist_id_name_pair.empty())
        {
            output.success = false;
            return output;
        }

        std::vector<int32_t> artist_ids;

        for (int i = 0; i < artist_id_name_pair.size(); i++)
        {   
            std::cout << i << ". " << artist_id_name_pair[i].second << '\n';
        }

        int index_input = _get_bounded_numerical_int(0, artist_id_name_pair.size() - 1);
        if (index_input == -1)
        {
            output.success = false;
            return output;
        }
        output.artist_id = artist_id_name_pair[index_input].first;
    }

    output.success = true;
    output.action = static_cast<Actions>(action_input);
    return output;
}

int InputManager::_get_bounded_numerical_int(int lower, int upper)
{
    int final_input;
    std::string raw_input;
    std::getline(std::cin, raw_input);

    if (!Utilities::parse_number(raw_input, final_input) || final_input < lower || final_input > upper)
    {
        std::cout << '\n';
        LOG_WARN("Selection out of allowed range");
        return -1;
    }

    return final_input;
}

std::string InputManager::get_string_input(const std::string& prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input;
}

int32_t InputManager::get_numerical_input(const std::string& prompt)
{
    std::cout << prompt;

    int final_input;
    std::string raw_input;
    std::getline(std::cin, raw_input);
    
    if (!Utilities::parse_number(raw_input, final_input))
    {
        LOG_WARN("Invalid attribute selection");
        return -1;
    }

    return final_input;
}

double InputManager::get_decimal_input(const std::string& prompt)
{
    std::cout << prompt;

    double final_input;
    std::string raw_input;
    std::getline(std::cin, raw_input);

    if (!Utilities::parse_number(raw_input, final_input))
    {
        LOG_WARN("Invalid attribute selection");
        return -1.0;
    }

    return final_input;
}

bool InputManager::get_bool_input(const std::string& prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input.at(0) == 'y';
}

ArtistRoles InputManager::get_role_input(const std::string& prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    std::transform(input.begin(), input.end(), input.begin(),
        [](unsigned char c)
        { return static_cast<char>(std::tolower(c));
    });

    if (input == "h" || input == "headline")
    {
        return ArtistRoles::Headline;
    }
    else if (input == "s" || input == "support")
    {
        return ArtistRoles::Support;
    }
    else if (input == "g" || input == "guest")
    {
        return ArtistRoles::Guest;
    }
    else
    {
        return ArtistRoles::None;
    }
}
