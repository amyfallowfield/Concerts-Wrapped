cmake_minimum_required(VERSION 3.20)

project(ConcertWrapped)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(concert-wrapped
    src/Main.cpp
    src/models/src/Artist.cpp
    src/models/src/Concert.cpp
    src/models/src/Performance.cpp
    src/repositories/src/ConcertRepository.cpp
    src/screens/src/ScreenManager.cpp
    src/services/src/ConcertStatsManager.cpp
    src/services/src/Utilities.cpp
    src/services/src/ValidationManager.cpp
    src/services/src/ArtistStatsManager.cpp
    src/services/src/InputManager.cpp
    src/services/src/Logger.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

target_link_libraries(concert-wrapped PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(concert-wrapped PRIVATE
    src/models/include
    src/repositories/include
    src/screens/include
    src/services/include
)

add_executable(unit-tests
    tests/helpers/TestData.cpp
    tests/models/ArtistTest.cpp
    tests/models/ConcertTest.cpp
    tests/models/PerformanceTest.cpp
    tests/screens/ScreenManagerTest.cpp
    src/models/src/Artist.cpp
    src/models/src/Concert.cpp
    src/models/src/Performance.cpp
    src/repositories/src/ConcertRepository.cpp
    src/screens/src/ScreenManager.cpp
    src/services/src/ConcertStatsManager.cpp
    src/services/src/Utilities.cpp
    src/services/src/ValidationManager.cpp
    src/services/src/ArtistStatsManager.cpp
    src/services/src/InputManager.cpp
    src/services/src/Logger.cpp
)

target_include_directories(unit-tests PRIVATE
    tests/helpers
    tests/models
    tests/repositories
    tests/screens
    tests/services
    src/models/include
    src/repositories/include
    src/screens/include
    src/services/include
)

target_link_libraries(unit-tests PRIVATE
    GTest::gmock_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(unit-tests)
