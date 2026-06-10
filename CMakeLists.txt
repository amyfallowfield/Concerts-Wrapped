cmake_minimum_required(VERSION 3.20)

project(ConcertWrapped)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(concert-wrapped
    src/Main.cpp
    src/models/src/Artist.cpp
    src/models/src/Concert.cpp
    src/repositories/src/ConcertRepository.cpp
    src/services/src/ConcertStatsManager.cpp
    src/services/src/Utilities.cpp
    src/services/src/ValidationManager.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(concert-wrapped PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(concert-wrapped PRIVATE
    src/models/include
    src/repositories/include
    src/services/include
)
