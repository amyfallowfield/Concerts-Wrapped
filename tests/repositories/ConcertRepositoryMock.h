#ifndef MOCK_CONCERT_REPOSITORY_H
#define MOCK_CONCERT_REPOSITORY_H

#include <gmock/gmock.h>

#include "ConcertRepository.h"
#include "StorageManager.h"

class ConcertRepositoryMock : public ConcertRepository
{
  public:
    ConcertRepositoryMock(StorageManager &storage) : ConcertRepository(storage)
    {
    }

    MOCK_METHOD(void, add_concert,
                ((std::map<std::string, ArtistRoles>), std::string, std::string, std::string,
                 double),
                (override));
    MOCK_METHOD(void, remove_concert, (int32_t concert_id), (override));
    MOCK_METHOD(void, add_artist, (int32_t concert_id, std::string artist_name, ArtistRoles role),
                (override));
    MOCK_METHOD(void, update_venue, (int32_t concert_id, std::string venue), (override));
    MOCK_METHOD(void, update_city, (int32_t concert_id, std::string city), (override));
    MOCK_METHOD(void, update_date, (int32_t concert_id, std::string date), (override));
    MOCK_METHOD(void, update_cost, (int32_t concert_id, double cost), (override));
};

#endif
