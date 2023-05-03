#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <memory>

#include "my_http_lib/client.h"

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

bool ProcessInputs(std::string msg, Request &request);
std::string ToLowerString(std::string data);

int main(int argc, char *argv[]) {

    std::shared_ptr<Client> client;
    bool ConnectionValid = false;
    do{
        std::cout << "To connect to the server, please enter the address. Ex : 127.0.0.1"<< std::endl;
        std::string temp;
/*        std::cin >> temp;
        const std::string serverIP = temp;*/
        const std::string serverIP = "127.0.0.1";
        uint16_t serverPort = 45'000;
        try {
            client = std::make_shared<Client>(serverIP, serverPort);
            ConnectionValid = true;
        }
        catch (std::string errorMsg){
            std::cout << errorMsg;
            ConnectionValid = false;
        }
    } while (!ConnectionValid);

    std::cout << "Enter \"QUIT\" to close the program" << std::endl;
    std::cout << "Please format your request like this :" << std::endl;
    std::cout << "Verb/Path/Argument" << std::endl;
    std::cout << "Ex : Get/home/Argument" << std::endl;
    bool stop = false;
    while(!stop){
        std::cout << "Enter the message you want to send : ";
        Request request = Request(Method::Get, "path", "message");
        //Lire les messages depuis la console
        std::string messageToSend;

        std::cin >> messageToSend;

        if(messageToSend != ToLowerString("QUIT")){
            if(ProcessInputs(messageToSend, request)){
                client->LaunchRequest(request);
            }
        } else{
            // Mettre le boolean stop à true et close la socket
            // TODO : Close Socket
            delete &client;

            stop = true;
            std::cout << "Closing ... " << std::endl;
        }
    }

    //Create a client allowing to do request on certain URL (see the PDF at the root)
    return EXIT_SUCCESS;
}

bool ProcessInputs(std::string msg, Request &request){
    bool validRequest = false;
    std::stringstream test(msg);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, '/'))
    {
        seglist.push_back(segment);
    }

    if(seglist.size() == 3){
        Method tempMethod = char_to_enum(ToLowerString(seglist[0]).c_str());
        if(tempMethod != Method::Unknown){
            request.SetMethod(tempMethod);
            validRequest = true;
            std::cout << "Not Unknown" << std::endl;
        }
        request.SetPath("/" + seglist[1]);
        request.SetBody(seglist[2]);
    }else{
        std::cout << "Wrong request format, make sure your request is formatted Verb/Path/Argument" << std::endl;
    }


    return validRequest;
}

std::string ToLowerString(std::string data){
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return data;
}