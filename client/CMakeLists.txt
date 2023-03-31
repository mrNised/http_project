set(CLIENT_HEADERS)

set(CLIENT_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(client ${CLIENT_HEADERS} ${CLIENT_SRC})
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(client PUBLIC my_http_lib)