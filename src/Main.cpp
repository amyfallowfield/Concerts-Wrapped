#include <iostream>
#include <limits>
#include <vector>

#include "Utilities.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "ConcertStatsManager.h"
#include "StorageManager.h"

int main()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";

    ConcertStatsManager stats = ConcertStatsManager();
    StorageManager storage = StorageManager();
    ConcertRepository repo = ConcertRepository();

    while (true)
    {
        int input;

        std::cout << "1. Add Concert\n";
        std::cout << "2. View Concerts\n";
        std::cout << "3. View Stats\n";
        std::cout << "4. Exit\n";
        std::cout << "Selection: ";

        if (!Utilities::parse_int(input) || input < 1 || input > 4)
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
            stats.print_stats(repo.get_concerts());
            break;
        }
        case 4:
        {
            storage.save(repo.get_concerts());
            storage.save(repo.get_artists());
            return 0;
        }
        }

        std::cout << '\n';
    }
}