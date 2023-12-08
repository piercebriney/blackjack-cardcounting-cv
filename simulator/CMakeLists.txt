cmake_minimum_required(VERSION 3.19)

project(simulator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(simulator analysis.cpp dealer.cpp matrix.cpp shoe.cpp common.cpp main.cpp player.cpp strategy.cpp)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/morrisfranken/argparse
)
FetchContent_MakeAvailable(argparse)

find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX argparse)
