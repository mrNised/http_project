#include <iostream>

#include <my_http_lib/http_lib.h>
#include "my_http_lib/server.h"

#ifdef WINDOWS_PLATFORM
//We use a struct to always to WSAStartup and Cleanup only once in the
//lifetime of the program. The static variables below will be created at the program startup
//calling the struct constructor, initializing WinSock. And the variable will be destructed
//at the end of the program calling WASCleanup()
struct WinSockInitializer
{
    WinSockInitializer()
    {
        //On Windows only you need to initialize the Socket Library in version 2.2
        WSAData wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);
    }

    ~WinSockInitializer()
    {
        WSACleanup();
    }
};

static WinSockInitializer __WIN_INIT__ {};
#endif


using namespace my_http_lib;

int main(int argc, char *argv[]) {

    /*my_http_lib::test();*/
    std::cout << "Hello, World!" << std::endl;
    //Create a server serving HTTP content (see the PDF for more infos)
    const std::string listeningInterface = "127.0.0.1";
    uint16_t listeningPort = 45'000;
    my_http_lib::Server server = Server(listeningInterface, listeningPort);
    return EXIT_SUCCESS;
}
