#include <iostream>
#include <limits>
#include <vector>

#include "models/Concert.h"
#include "repositories/ConcertRepository.h"
#include "services/StorageManager.h"
#include "Utilities.h"

int main()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";
    StorageManager storage = StorageManager();
    storage.load();

    ConcertRepository repo = ConcertRepository();

    while (true)
    {
        int input;

        std::cout << "1. Add Concert\n";
        std::cout << "2. View Concerts\n";
        std::cout << "3. Exit\n";

        if (!Utilities::parse_int(input) || input < 1 || input > 3)
            continue;

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
            storage.save(repo.get_all());
            return 0;
        }
        }
    }
}