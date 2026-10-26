#ifndef CONCERT_ARTIST_H
#define CONCERT_ARTIST_H

#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ArtistRoles
{
    Headline,
    Support,
    Guest,
    None
};

class ConcertArtist
{
public:
    ConcertArtist(int32_t concert_id, int32_t artist_id, ArtistRoles role);
    ConcertArtist(const json &data);

    json to_json() const;

    int32_t get_concert_id() const;
    int32_t get_artist_id() const;
    ArtistRoles get_role() const;

    void set_concert_id(int32_t id);
    void set_artist_id(int32_t id);
    void set_role(ArtistRoles role);

private:
    int32_t concert_id;
    int32_t artist_id;
    ArtistRoles artist_role;
};

#endif
