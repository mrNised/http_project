#pragma once

#include <llhttp.h>

#include <WinSock2.h>
#include "response.h"
#include "request.h"

namespace my_http_parser {
    class Client
    {
    public:
        /**
         * Creates a client, connecting it to a server
         * @param ClientSocket The client's socket
         */
        explicit Client(const std::string sevrerIp, uint16_t serverPort);

        /**
         * Destroy a client
         */
        ~Client();

        /**
         * Launch a request and block until a response is received
         * @param request
         * @return
         */
        Response LaunchRequest(const Request& request);
    private:
        llhttp_t* m_parser;
        SOCKET m_clientSocket;

        Response m_currentResponse;

        /**
         * Called when the message coming from the server is parsed
         * @param m_parser THe parser to use
         * @return
         */
        static int HandleOnMessageComplete(llhttp_t* m_parser);

        /**
         * Called when the status of the response is parsed
         * @param m_parser
         * @return
         */
        static int HandleStatusParsed(llhttp_t* m_parser);

        /**
         * Called when the body of the response is parsed
         * @param m_parser
         * @return
         */
        static int HandleBodyParsed(llhttp_t* m_parser);
    };
}