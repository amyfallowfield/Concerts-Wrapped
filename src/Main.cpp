#include <iostream>
#include <limits>
#include <vector>

#include "Utilities.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "ArtistStatsManager.h"
#include "ConcertStatsManager.h"
#include "StorageManager.h"

int main()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";

    ArtistStatsManager artist_stats = ArtistStatsManager();
    ConcertStatsManager concert_stats = ConcertStatsManager();
    StorageManager storage = StorageManager();
    ConcertRepository repo = ConcertRepository();

    while (true)
    {
        int input;

        std::cout << "1. Add Concert\n";
        std::cout << "2. View All Concerts\n";
        std::cout << "3. View Concert Stats\n";
        std::cout << "4. View Artist Stats\n";
        std::cout << "5. Exit\n";
        std::cout << "Selection: ";

        if (!Utilities::parse_int(input) || input < 1 || input > 5)
            continue;

        std::cout << '\n';

        switch(input)
        {
        case 1:
        {
            repo.add();
            break;
        }
        case 2:
        {
            repo.print();
            break;
        }
        case 3:
        {
            concert_stats.print_stats(repo.get_concerts());
            break;
        }
        case 4:
        {
            artist_stats.print_stats(repo.get_artists());
            break;
        }
        case 5:
        {
            storage.save(repo.get_concerts());
            storage.save(repo.get_artists());
            return 0;
        }
        }

        std::cout << '\n';
    }
}