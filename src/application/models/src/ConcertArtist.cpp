#include <stdexcept>
#include <string>

#include "ConcertArtist.h"

using json = nlohmann::json;

namespace
{
std::string role_to_string(ArtistRoles role)
{
    switch (role)
    {
    case ArtistRoles::Headline:
        return "Headline";
    case ArtistRoles::Support:
        return "Support";
    case ArtistRoles::Guest:
        return "Guest";
    case ArtistRoles::None:
        break;
    }
    throw std::invalid_argument("Concert artist role must be Headline, Support, or Guest");
}

ArtistRoles role_from_string(const std::string &role)
{
    if (role == "Headline")
    {
        return ArtistRoles::Headline;
    }
    if (role == "Support")
    {
        return ArtistRoles::Support;
    }
    if (role == "Guest")
    {
        return ArtistRoles::Guest;
    }
    throw std::invalid_argument("Unknown concert artist role: " + role);
}
} // namespace

ConcertArtist::ConcertArtist(int32_t concert_id, int32_t artist_id, ArtistRoles role)
    : concert_id(concert_id), artist_id(artist_id), artist_role(role)
{
}

ConcertArtist::ConcertArtist(const json &data)
    : concert_id(data.at("concert_id")), artist_id(data.at("artist_id")),
      artist_role(role_from_string(data.at("role").get<std::string>()))
{
}

json ConcertArtist::to_json() const
{
    return {{"concert_id", concert_id},
            {"artist_id", artist_id},
            {"role", role_to_string(artist_role)}};
}

int32_t ConcertArtist::get_concert_id() const
{
    return concert_id;
}

int32_t ConcertArtist::get_artist_id() const
{
    return artist_id;
}

ArtistRoles ConcertArtist::get_role() const
{
    return artist_role;
}

void ConcertArtist::set_concert_id(int32_t id)
{
    concert_id = id;
}

void ConcertArtist::set_artist_id(int32_t id)
{
    artist_id = id;
}

void ConcertArtist::set_role(ArtistRoles role)
{
    artist_role = role;
}
