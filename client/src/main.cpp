#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

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
Method string_to_enum(std::string msg);

static std::unordered_map<std::string,Method> const VerbEnumMap = {
        {"get",Method::Get},
        {"post",Method::Post},
        {"put",Method::Put},
        {"delete",Method::Delete},
        {"option",Method::Option}
};

int main(int argc, char *argv[]) {

    std::cout << "Hello, World!" << std::endl;
    const std::string serverIP = "127.0.0.1";
    uint16_t serverPort = 45'000;
    /*Client clientTest = Client(serverIP, serverPort);*/

    bool stop = false;
    while(!stop){
        Request request = Request(Method::Get, "path", "message");
        //Lire les messages depuis la console
        std::string messageToSend;
        std::cout << "Enter \"QUIT\" to close the program" << std::endl;
        std::cout << "Please format your request like this :" << std::endl;
        std::cout << "Verb/Path/Argument" << std::endl;
        std::cout << "Ex : Get/home/Argument" << std::endl;
        std::cout << "Enter the message you want to send : ";
        std::cin >> messageToSend;

        if(messageToSend == ToLowerString("QUIT")){
            // Mettre le boolean stop à true et close la socket
            // TODO : Close Socket
            stop = true;
            std::cout << "Closing ... " << std::endl;
        } else{
            ProcessInputs(messageToSend, request);
        }
        //Envoyer le message au serveur
        /*clientTest.TestLaunch(messageToSend);*/

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

    Method tempMethod = string_to_enum(ToLowerString(seglist[0]));
    if(tempMethod != Method::Unknown){
        request.SetMethod(tempMethod);
        validRequest = true;
        std::cout << "Not Unknown" << std::endl;
    }

    return validRequest;
}

std::string ToLowerString(std::string data){
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return data;
}

Method string_to_enum(std::string msg) {
    Method method;
    auto it = VerbEnumMap.find(msg);
    if (it != VerbEnumMap.end()) {
        method = it->second;
        std::cout << "Good answer" << std::endl;
    } else {
        std::cout << "There was an error while finding the verb" << std::endl;
        method = Method::Unknown;
    }
    return method;
}
