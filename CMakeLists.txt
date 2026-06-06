cmake_minimum_required(VERSION 3.20)

project(ConcertWrapped)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(concert-wrapped
    src/main.cpp
    src/Utilities.cpp
    src/models/Concert.cpp
    src/repositories/ConcertRepository.cpp
    src/services/StorageManager.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(concert-wrapped PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(concert-wrapped PRIVATE src)