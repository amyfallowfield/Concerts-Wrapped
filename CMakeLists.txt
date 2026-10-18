cmake_minimum_required(VERSION 3.20)

project(ConcertWrapped)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(concert-wrapped
    src/Main.cpp
    src/application/models/src/Artist.cpp
    src/application/models/src/Concert.cpp
    src/application/repositories/src/ConcertRepository.cpp
    src/application/screens/src/screenManager.cpp
    src/application/services/src/Logger.cpp
    src/application/services/src/StatsManager.cpp
    src/application/services/src/ValidationManager.cpp
    src/input/src/ConsoleInput.cpp
    src/presentation/src/ConsoleConcertsView.cpp
    src/presentation/src/ConsoleMenuView.cpp
    src/presentation/src/ConsoleStatsView.cpp
    src/presentation/src/ConsolePopUpView.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

target_link_libraries(concert-wrapped PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(concert-wrapped PRIVATE
    src
    src/application/models/include
    src/application/repositories/include
    src/application/screens/include
    src/application/services/include
    src/input/include
    src/presentation/include
)

add_executable(unit-tests
    src/application/models/src/Artist.cpp
    src/application/models/src/Concert.cpp
    src/application/repositories/src/ConcertRepository.cpp
    src/application/screens/src/ScreenManager.cpp
    src/application/services/src/Logger.cpp
    src/application/services/src/StatsManager.cpp
    src/application/services/src/ValidationManager.cpp
    src/input/src/ConsoleInput.cpp
    src/presentation/src/ConsoleConcertsView.cpp
    src/presentation/src/ConsoleMenuView.cpp
    src/presentation/src/ConsoleStatsView.cpp
    src/presentation/src/ConsolePopUpView.cpp
    tests/helpers/TestData.cpp
    tests/models/ArtistTest.cpp
    tests/models/ConcertTest.cpp
    tests/repositories/ConcertRepositoryTest.cpp
    tests/screens/ScreenManagerTest.cpp
    tests/services/ConsoleInputTest.cpp
    tests/services/LoggerTest.cpp
    tests/services/StatsManagerTest.cpp
    tests/services/StorageManagerTest.cpp
    tests/services/ValidationManagerTest.cpp
)

target_include_directories(unit-tests PRIVATE
    src
    src/application/models/include
    src/application/repositories/include
    src/application/screens/include
    src/application/services/include
    src/input/include
    src/presentation/include
    tests/helpers
    tests/models
    tests/repositories
    tests/screens
    tests/services
)

target_link_libraries(unit-tests PRIVATE
    GTest::gmock_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(unit-tests)
