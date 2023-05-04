#include <ws2tcpip.h>
#include <iostream>
#include "my_http_lib/client.h"
#include "fmt/format.h"

namespace my_http_lib {
    Client::Client(const std::string sevrerIp, uint16_t serverPort) {
        //1 - Connect the client socket to the server

        //We will need a addrinfo to store the result of the DNS
        //request that we make using getaddrinfo
        addrinfo* result = nullptr;

        //A request DNS needs hints to resolve the best address avaiable
        addrinfo hints {};
        memset(&hints, 0, sizeof(hints));
        //We want an IPv4
        hints.ai_family = AF_INET;
        //We want a TCP socket
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        //We make a DNS request asking for the address of localhost on port 45000
        //This request store the information in the result
        int error = getaddrinfo(sevrerIp.c_str(), std::to_string(serverPort).c_str(), &hints, &result);

        if(error < 0)
        {
            error = WSAGetLastError();
            throw (fmt::format("There was an error {} while {}", error, "the client tried getting the address"));
        }

        //Addrinfo (returned by getaddrinfo) is linked list of address.
        //We need to go through it trying to connect to any address returned. Until we find
        //one that is good for us
        m_clientSocket = INVALID_SOCKET;
        for(auto* ptr = result; ptr != nullptr ; ptr = ptr->ai_next)
        {
            //We create a matching socket with the one getaddrinfo found
            m_clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

            //Connect asks the OS to connect our socket to a server
            //pass in parameters. This connection will be accepted by
            //the server when this one calls "accept"
            error = connect(m_clientSocket,
                            ptr->ai_addr,
                            ptr->ai_addrlen);

            if(error < 0)
            {
                error = WSAGetLastError();
                std::cout << "There was an error" << std::endl;
                closesocket(m_clientSocket);
                m_clientSocket = INVALID_SOCKET;
                continue;
            }

            break;
        }

        if(m_clientSocket == INVALID_SOCKET)
        {
            //No usable address to connect to.
            std::cout << "Host unreachable" << std::endl;
        }

        //2 - Initialize the llhttp parser using llhttp_settings_t like in the example from https://github.com/nodejs/llhttp README
        //          - Register our callbacks
        //              - static int HandleOnMessageComplete(llhttp_t* m_parser);
        //              - static int HandleStatusParsed(llhttp_t* m_parser);
        //              - static int HandleBodyParsed(llhttp_t* m_parser);
        //         - Once the parser is initialized, set this at the parser data this will allow to get this later in our callbacks
        //              - m_parser->data = this;

        llhttp_settings_t settings;
        /* Initialize user callbacks and settings */
        llhttp_settings_init(&settings);

        /* Set user callback */
        settings.on_status_complete = HandleStatusParsed;
        settings.on_body = HandleBodyParsed;

        /* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
         * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
         * input.
         */
        llhttp_init(&m_parser, HTTP_BOTH, &settings);

        m_parser.data = this;
    }

    Client::~Client() {
        //TODO - Close it the right way
        //1 - Close the socket
        closesocket(m_clientSocket);
        //3 - Delete any pointer
    }

    Response Client::LaunchRequest(const Request &request)
    {
        // 1 - Convert the request object to a char*
        //          - Remember how the HTTP request is done : https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
        //          - You just need basic stuff so no headers. Just method, path, version and body
        //          - Remember that a HTTP request MUST end with /r/n/r/n
        auto it = MethodToStringMap.find(request.GetMethod());
        std::string requestString = fmt::format("{} / HTTP/1.1\r\n\r\n", it->second);
        // 2 - Send the new request to the server
        int byteSent = send(m_clientSocket, requestString.c_str(), requestString.size(), 0);
        int error;
        if(byteSent < 0)
        {
            error = WSAGetLastError();
            closesocket(m_clientSocket);
            throw (fmt::format("There was an error {} while {}", error, "the client tried getting the address"));
        }
        // 3 - Recv until you get an answer
        std::array<char, 65535> recvBuffer{};
        int byteReceived = 0;
        byteReceived = recv(m_clientSocket, recvBuffer.data(), recvBuffer.size(), 0);

        if (byteReceived > 0)
        {
            //We received some data

            std::string msgReceived(recvBuffer.data(), byteReceived);
            std::cout << "CLIENT : We received : " << msgReceived << std::endl;

            // 4 - Start the parsing using llhttp and wait for it to finish (HandleOnMessageComplete method will help)
            ParseResponseFromServer(msgReceived);

        }
        else if (byteReceived == 0)
        {
            //The socket is closing
            closesocket(m_clientSocket);
            std::cout << "Socket closing..." << std::endl;
        }
        else
        {
            //- If RECV fail consider the client closed so call m_closeHandler
            error = WSAGetLastError();
            throw (fmt::format("There was an error {} while {}", error, "connection tried recv"));
        }

        // 5 - Returns the parsed response
        return m_currentResponse;
    }

    int Client::HandleStatusParsed(llhttp_t *m_parser) {
        // 1 - Fill the status in the object response (m_currentResponse) (use m_parser->data to get back your this pointer)
        Client* connection = reinterpret_cast<Client*>(m_parser->data);
        connection->m_currentResponse.SetCode(llhttp_get_status_code(m_parser));
        return 0;
    }

    int Client::HandleBodyParsed(llhttp_t *m_parser, const char *at, size_t length) {
        // 1 - Fill the body in the object response (m_currentResponse) (use m_parser->data to get back your this pointer)
        Client* connection = reinterpret_cast<Client*>(m_parser->data);
        connection->m_currentResponse.SetBody(std::string(at, length));
        return 0;
    }

    void Client::ParseResponseFromServer(std::string msgReceived) {
        /* Parse response! */

        enum llhttp_errno err = llhttp_execute(&m_parser, msgReceived.c_str(), msgReceived.size());
        if (err == HPE_OK) {
            /* Successfully parsed! */
        } else {
            fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                    m_parser.reason);
        }
    }

    void Client::TestLaunch(std::string message) {
        //Now we are connected to the server
        int byteSent = send(m_clientSocket, message.c_str(), strlen(message.c_str()), 0);

        int error;
        if(byteSent < 0)
        {
            error = WSAGetLastError();
            std::cout << "There was an error : " << error << std::endl;
            closesocket(m_clientSocket);
        }
    }
}