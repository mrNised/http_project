#include <iostream>

#include <my_http_lib/http_lib.h>

int main(int argc, char* argv[])
{
    my_http_lib::test();
    std::cout << "Hello, World!" << std::endl;
    //Create a client allowing to do request on certain URL (see the PDF at the root)
    return EXIT_SUCCESS;
}