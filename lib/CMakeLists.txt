set(MY_HTTP_LIB_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/http_lib.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/client.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/method.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/request.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/response.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/server.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/connection.h)

set(MY_HTTP_LIB_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/http_lib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/method.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/request.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/connection.cpp)

add_library(my_http_lib STATIC ${MY_HTTP_LIB_HEADERS} ${MY_HTTP_LIB_SRC})
target_include_directories(my_http_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(my_http_lib PUBLIC fmt::fmt)
target_link_libraries(my_http_lib PUBLIC llhttp_static)
target_link_libraries(my_http_lib PUBLIC nlohmann_json::nlohmann_json)