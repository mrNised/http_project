#pragma once

#include <llhttp.h>

#include <WinSock2.h>
#include "response.h"
#include "request.h"

namespace my_http_lib {
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
       * For testing
       */
        void TestLaunch(std::string message);

        /**
         * Launch a request and block until a response is received
         * @param request
         * @return
         */
        Response LaunchRequest(const Request& request);
    private:
        llhttp_t m_parser;
        SOCKET m_clientSocket;

        Response m_currentResponse;

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
        static int HandleBodyParsed(llhttp_t* m_parser, const char *at, size_t length);

        /**
         * Called to parse the response from the server
         * @param msgReceived
         */
        void ParseResponseFromServer(std::string msgReceived);
    };
}