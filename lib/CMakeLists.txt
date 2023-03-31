set(MY_HTTP_LIB_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/my_http_lib/http_lib.h)

set(MY_HTTP_LIB_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/http_lib.cpp)

add_library(my_http_lib STATIC ${MY_HTTP_LIB_HEADERS} ${MY_HTTP_LIB_SRC})
target_include_directories(my_http_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)