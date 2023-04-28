#include <iostream>
#include <wspiapi.h>

#include "my_http_lib/server.h"

namespace my_http_lib
{
    Server::Server(const std::string &listeningInterface, uint16_t listeningPort)
    {
        //1 - Open a socket on the specific interface (it is an IPv4 address) and port
        m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        inet_addr(listeningInterface.c_str());


        //sockaddr_in represents a IPv4 address.
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(listeningPort);
        serverAddr.sin_addr.S_un.S_addr = INADDR_ANY/*inet_addr(listeningInterface.c_str())*/;

        //Once a TCP socket is created,
        // => On a server we must bind to a port and an IP
        int error = bind(m_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
        if(error < 0)
        {
            error = WSAGetLastError();
            std::cout << "There was an error : " << error << std::endl;
            //TODO : Error
        }
    }

    Server::~Server()
    {
        // 1- Close the socket
        //We close our server
        closesocket(m_serverSocket);
        //TODO 2 - Close all the clients connections
        //We close our client socket
        /*closesocket(clientSocket);*/
        // 3 - Cleanup everything
    }

    void Server::Listen()
    {
        //1 - In loop (blocking until the server is closed)
        //          - Accept new connection from the TCP socket
        //          - For each new connection create a Connection object with the callbacks filled
        //          - Add the Connection object to the list of all the connections

        IsServerClosed = false;

        while (!IsServerClosed){

            //A server must listen for incoming packets
            int error = listen(m_serverSocket, 255);
            if(error < 0)
            {
                error = WSAGetLastError();
                std::cout << "There was an error : " << error << std::endl;
                //TODO : Error
            }

            //TCP is connection-based. Consequently, we must accept the new connection
            //before being able to talk to a client.
            sockaddr_in clientAddr;
            int clientLength = sizeof clientAddr;
            //memset = Memory Set, we are setting all the memory of clientAddr at 0
            memset(&clientAddr, 0, sizeof clientAddr);

            //Accept accepts a new connection coming from a client. This means that it
            //accepts and acknowledge the "connect" the client made. This function
            //returns a new socket that is used to communicate with this specific
            //client. The parameters are :
            // - The server socket
            // - A pointer to a sockaddr so we can store the client address
            // - A pointer to a size so we can store the client address size
            SOCKET clientSocket = accept(
                    m_serverSocket,
                    reinterpret_cast<sockaddr*>(&clientAddr),
                    &clientLength);

            //Accept on Windows returns an INVALID_SOCKET when there is an error
            if(clientSocket == INVALID_SOCKET)
            {
                error = WSAGetLastError();
                std::cout << "There was an error : " << error << std::endl;
                closesocket(m_serverSocket);
                //TODO : Error
            }

            /*Connection NewConnection = Connection(clientSocket, )*/
        }

    }

    void Server::Handle(Method method, const std::string &path, Handler handler)
    {
        //Register an handler into the m_handlers map.
    }

    void Server::Get(const std::string &path, Handler handler)
    {
        Handle(Method::Get, path, handler);
    }

    void Server::Post(const std::string &path, Handler handler)
    {
        Handle(Method::Post, path, handler);
    }

    void Server::Put(const std::string &path, Handler handler)
    {
        Handle(Method::Put, path, handler);
    }

    void Server::Delete(const std::string &path, Handler handler)
    {
        Handle(Method::Delete, path, handler);
    }
}
