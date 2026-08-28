#ifndef MOCK_CONCERT_REPOSITORY_H
#define MOCK_CONCERT_REPOSITORY_H

#include <gmock/gmock.h>

#include "ConcertRepository.h"
#include "StorageManager.h"

class ConcertRepositoryMock : public ConcertRepository
{
public:
    ConcertRepositoryMock(StorageManager& storage)
    : ConcertRepository(storage)
    {}

    MOCK_METHOD(void, add, (), (override));
    MOCK_METHOD(void, remove, (), (override));
    MOCK_METHOD(void, edit, (), (override));
    MOCK_METHOD(void, print, (), (override));
};

#endif
