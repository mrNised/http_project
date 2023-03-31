cmake_minimum_required(VERSION 3.20)

project(http_project
    VERSION 0.0.1
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(externals)

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(server)