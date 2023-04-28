set(SERVER_HEADERS)

set(SERVER_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(server ${SERVER_HEADERS} ${SERVER_SRC})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(server PUBLIC my_http_lib ws2_32)

if(WIN32)
    target_compile_definitions(server PUBLIC WINDOWS_PLATFORM)
endif()