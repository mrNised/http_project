#include "my_http_lib/server.h"

namespace my_http_parser
{
    Server::Server(const std::string &listeningInterface, uint16_t listeningPort)
    {
        //1 - Open a socket on the specific interface (it is an IPv4 address) and port
    }

    Server::~Server()
    {
        // 1- Close the socket
        // 2 - Close all the clients connections
        // 3 - Cleanup everything
    }

    void Server::Listen()
    {
        //1 - In loop (blocking until the server is closed)
        //          - Accept new connection from the TCP socket
        //          - For each new connection create a Connection object with the callbacks filled
        //          - Add the Connection object to the list of all the connections
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
