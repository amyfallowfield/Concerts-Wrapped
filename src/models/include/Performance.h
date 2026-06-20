#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Performance
{
public:
    Performance(int show_id, std::string artist, std::string role);
    Performance(const json& data);

    json to_json() const;

private:
    int show_id;
    std::string artist;
    std::string role;   // TODO: Implement enum of roles: Headliner, Support, Suprise Guest
};

#endif
