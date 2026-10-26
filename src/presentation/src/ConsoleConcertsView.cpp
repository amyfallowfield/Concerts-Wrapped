#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "ConsoleConcertsView.h"

namespace
{
std::string display_role_name(ArtistRoles role)
{
    switch (role)
    {
    case ArtistRoles::Headline:
        return "Headliner";
    case ArtistRoles::Support:
        return "Support";
    case ArtistRoles::Guest:
        return "Guest";
    default:
        throw std::runtime_error("Artist role not recognised");
    }
}
} // namespace

void ConsoleConcertsView::show(const std::vector<Concert> &concerts,
                               const std::vector<Artist> &artists,
                               const std::vector<ConcertArtist> &concert_artists)
{
    for (const Concert &concert : concerts)
    {
        std::cout << "ID: " << concert.get_concert_id() << '\n';
        std::cout << "Artists:\n";
        for (const ConcertArtist &concert_artist : concert_artists)
        {
            if (concert_artist.get_concert_id() != concert.get_concert_id())
            {
                continue;
            }
            const int32_t artist_id = concert_artist.get_artist_id();
            auto artist = std::find_if(artists.begin(), artists.end(),
                                       [artist_id](const Artist &candidate)
                                       { return candidate.get_id() == artist_id; });
            if (artist != artists.end())
                std::cout << "- " << artist->get_name() << " ["
                          << display_role_name(concert_artist.get_role()) << "]\n";
        }
        std::cout << "Venue: " << concert.get_venue() << '\n'
                  << "City: " << concert.get_city() << '\n'
                  << "Date: " << concert.get_date() << '\n'
                  << "Cost: \xC2\xA3" << concert.get_cost() / 100.0 << "\n\n";
    }
}
