#include <iostream>

#include <fmt/format.h>

#include "my_http_lib/http_lib.h"


namespace my_http_lib
{

    void test()
    {
        std::cout << fmt::format("Hello World from {}", "toto")  << std::endl;
    }
}