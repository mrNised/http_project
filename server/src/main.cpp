#include <iostream>

#include <my_http_lib/http_lib.h>

int main(int argc, char* argv[])
{
    my_http_lib::test();
    std::cout << "Hello, World!" << std::endl;
    //Create a server serving HTTP content (see the PDF for more infos)
    return EXIT_SUCCESS;
}