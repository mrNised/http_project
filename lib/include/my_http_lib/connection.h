#pragma once

#include <functional>
#include <memory>
#include <thread>

#include <WinSock2.h>

#include <llhttp.h>

#include <my_http_lib/response.h>
#include <my_http_lib/request.h>

namespace my_http_parser
{
    class Connection
    {
    public:
        using ConnectionRequestHandler = std::function<void(const Request&, Response&)>;

        using ConnectionClosedHandler = std::function<void(const Connection& closedConnection)>;

        Connection(SOCKET clientSocket, ConnectionRequestHandler RequestHandler, ConnectionClosedHandler CloseHandler);
        ~Connection();

        /**
         * Close a connection cleaning up its socket
         */
        void Close();
    private:
        SOCKET                       m_clientSocket;
        std::shared_ptr<std::thread> m_clientThread;

        llhttp_t* m_parser;

        Request m_currentRequest;

        ConnectionRequestHandler m_requestHandler;

        ConnectionClosedHandler  m_closeHandler;

        /**
         * The method executed by the connection thread
         */
        void ThreadUpdate();

        /**
         * Called when the message coming from the client is parsed
         * @param m_parser THe parser to use
         * @return
         */
        static int HandleOnMessageComplete(llhttp_t* m_parser);

        /**
         * Called when the path of the client request is parsed
         * @param m_parser
         * @return
         */
        static int HandlePathParsed(llhttp_t* m_parser);

        /**
         * Called when the method of the client request is parsed
         * @param m_parser
         * @return
         */
        static int HandleMethodParsed(llhttp_t* m_parser);

        /**
         * Called when the body of the client request is parsed
         * @param m_parser
         * @return
         */
        static int HandleBodyParsed(llhttp_t* m_parser);
    };
}