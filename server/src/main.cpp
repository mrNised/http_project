#include <iostream>

#include <my_http_lib/http_lib.h>
#include "my_http_lib/server.h"
#include "fmt/format.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

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

using json = nlohmann::json;
using namespace my_http_lib;

int main(int argc, char *argv[])
{
    std::cout << "Hello, World!" << std::endl;
    //Create a server serving HTTP content (see the PDF for more infos)
    const std::string listeningInterface = "127.0.0.1";
    uint16_t listeningPort = 45'000;

    Server server = Server(listeningInterface, listeningPort);
    /*server.Get("/toto", [](const Request& request, Response& response){response.SetBody("Hello world!");});*/

    // Jsp comment faire que ça marche pour n'importe quel variable
    std::string VAR_NAME = "/toto";
    server.Get("/home", [](const Request& req, Response& resp){
        // Renvoi un compteur des vues qui ont déjà été fait sur /home
        static int NumberOfViews = 0;
        NumberOfViews++;
        resp.SetCode(200);
        resp.SetBody(fmt::format("{}", NumberOfViews));
    });
    server.Post(VAR_NAME, [](const Request& req, Response& resp){
        // Retourner en JSON le nombre de hit pour cette variable
        // {"name":"VAR_NAME","value": NOMBRE_HIT}
        // Si la variable existe, retourner 200 (Ok)
        // Si la variable n'existait pas, retourner 201 (Created)
        static int NumberOfViews = 0;
        if(NumberOfViews != 0){
            resp.SetCode(200);
            resp.SetBody(fmt::format("Ok"));
        } else{
            resp.SetCode(201);
            resp.SetBody(fmt::format("Created"));
        }
        NumberOfViews++;
    });
    server.Get(VAR_NAME, [](const Request& req, Response& resp){
        // Retourner en JSON le nombre de hit pour cette variable
        // {"name":"VAR_NAME","value": NOMBRE_HIT}
        // Si la variable n'existe pas, retourner 404 (Not found)
        static int NumberOfViews = 0;
        json j = {
                {"name", req.GetPath()},
                {"value",NumberOfViews}
        };

        if(NumberOfViews != 0){
            resp.SetCode(200);
            resp.SetBody(j.dump());
        }else{
            resp.SetCode(404);
            resp.SetBody(fmt::format("Not found"));
        }
    });
    server.Delete(VAR_NAME, [](const Request& req, Response& resp){
        // Mettre la valeur à 0
        // Si la variable n'existe pas, retourner 404 (Not found)
        static int NumberOfViews = 0;
        if(NumberOfViews != 0){
            resp.SetCode(200);
            resp.SetBody(fmt::format("Ok"));
        } else{
            resp.SetCode(404);
            resp.SetBody(fmt::format("Not found"));
        }
        NumberOfViews = 0;
    });
    server.Get("/index.html", [](const Request& req, Response& resp){
        // Retourner la page index.html qui est à la racine du dossier
        std::string line;
        std::string body;
        std::ifstream myfile("./index.html");
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                body += (line + '\n');
            }
            myfile.close();
            resp.SetCode(200);
            resp.SetBody(body);
        } else{
            resp.SetCode(404);
            resp.SetBody(fmt::format("Not found"));
        }
    });

    server.Listen();

    return EXIT_SUCCESS;
}
