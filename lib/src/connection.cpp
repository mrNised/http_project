#include <iostream>
#include "my_http_lib/connection.h"
#include "fmt/format.h"

namespace my_http_lib
{
    Connection::Connection(SOCKET clientSocket, Connection::ConnectionRequestHandler RequestHandler,
                           Connection::ConnectionClosedHandler CloseHandler):
                           m_clientSocket(clientSocket),
                           m_requestHandler(RequestHandler),
                           m_closeHandler(CloseHandler)

    {
        // 1 - Start the recv thread
        stop = false;
        m_clientThread = std::make_shared<std::thread>([&]{

            ThreadUpdate();
        });
        // 2 - Initialize the llhttp parser using llhttp_settings_t like in the example from https://github.com/nodejs/llhttp README
        //                  - Register our callbacks
        //                      - static int HandleOnMessageComplete(llhttp_t* m_parser);
        //                      - static int HandlePathParsed(llhttp_t* m_parser);
        //                      - static int HandleBodyParsed(llhttp_t* m_parser);
        //                      - static int HandleMethodParsed(llhttp_t* m_parser);
        //                 - Once the parser is initialized, set this at the parser data this will allow to get this later in our callbacks
        //                      - m_parser->data = this;
        llhttp_settings_t settings;
        /* Initialize user callbacks and settings */
        llhttp_settings_init(&settings);

        /* Set user callback */
        settings.on_url = &HandlePathParsing;
        settings.on_url_complete = &HandlePathParsed;
        settings.on_body = &HandleBodyParsed;
        settings.on_method = &HandleMethodParsing;
        /*settings.on_method_complete = &HandleMethodParsed;*/


        m_parser.data = this;
        llhttp_init(&m_parser, HTTP_BOTH, &settings);
    }

    Connection::~Connection()
    {
        Close();
    }

    void Connection::Close()
    {
        //1 - Kill the thread
        //ATTENTION : en C++ un thread doit être join avant d'être détruit
        //regarde les fonctions is_joinable() et join() avant de quitter la main
        stop = true;
        if(m_clientThread->joinable()){
            m_clientThread->join();
        }
        //2 - Close the socket
        closesocket(m_clientSocket);
        m_closeHandler(*this);
        //4 - Delete any pointer
    }

    /*int Connection::HandleOnMessageComplete(llhttp_t *m_parser)
    {
        // 1 - Stop the wait in ThreadUpdate (use m_parser->data to get back your this pointer)
        *//*Connection* connection = reinterpret_cast<Connection*>(m_parser->data);*//*

        return 0;
    }*/

    int Connection::HandlePathParsed(llhttp_t *m_parser)
    {
        // 1 - Fill the path in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        Connection* connection = reinterpret_cast<Connection*>(m_parser->data);
        connection->m_currentRequest.SetPath(connection->currentPath);
        return 0;
    }

    int Connection::HandlePathParsing(llhttp_t *m_parser, const char *at, size_t length)
    {
        Connection* connection = reinterpret_cast<Connection*>(m_parser->data);
        connection->currentPath += std::string(at, length);
        return 0;
    }

    int Connection::HandleMethodParsed(llhttp_t *m_parser)
    {
        // 1 - Fill the method in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        Connection* connection = reinterpret_cast<Connection*>(m_parser->data);
        Method temp = char_to_enum(connection->currentMethod);
        connection->m_currentRequest.SetMethod(temp);
        return 0;
    }

    int Connection::HandleMethodParsing(llhttp_t* m_parser, const char *at, size_t length)
    {
        Connection* connection = reinterpret_cast<Connection*>(m_parser->data);
        connection->currentMethod = strcat(connection->currentMethod, at);
        return 0;
    }

    int Connection::HandleBodyParsed(llhttp_t *m_parser, const char *at, size_t length)
    {
        // 1 - Fill the body in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        Connection* connection = reinterpret_cast<Connection*>(m_parser->data);
        connection->m_currentRequest.SetBody(std::string(at, length));
        return 0;
    }

    void Connection::ThreadUpdate()
    {

        int error;
        std::array<char, 65535> recvBuffer{};
        int byteReceived = 0;
        // 1 - Until the connection is closed :
        //          - Recv
        //          - Parse the request from the client
        //          - Call m_requestHandler with the parsed request and a response
        //          - Send the response to the client :
        //              - Remember how the HTTP message is done : https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
        //          - If RECV fail consider the client closed so call m_closeHandler
        while(!stop){
            byteReceived = recv(m_clientSocket, recvBuffer.data(), recvBuffer.size(), 0);

            if (byteReceived > 0)
            {
                //We received some data

                std::string msgReceived(recvBuffer.data(), byteReceived);
                std::cout << "SERVER : We received : " << msgReceived << std::endl;

                /* Parse request! */
                ParseRequestFromClient(msgReceived);


            }
            else if (byteReceived == 0)
            {
                //The socket is closing
                Close();
                std::cout << "Socket closing..." << std::endl;
            }
            else
            {
                //- If RECV fail consider the client closed so call m_closeHandler
                error = WSAGetLastError();
                m_closeHandler(*this);
                throw (fmt::format("There was an error {} while {}", error, "connection tried recv"));
            }
        }
    }

    void Connection::ParseRequestFromClient(std::string msgReceived) {
        /* Parse request! */

        enum llhttp_errno err = llhttp_execute(&m_parser, msgReceived.c_str(), msgReceived.size());
        if (err == HPE_OK) {
            /* Successfully parsed! */
            Response response;
                m_requestHandler(m_currentRequest, response);

                std::string responseString = fmt::format("HTTP/1.1 {} {}\n\n{}\n\r\n\r\n", response.GetCode(), "OK", response.GetBody());

                send(m_clientSocket, responseString.c_str(), responseString.size(), 0);
        } else {
            fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                    m_parser.reason);
        }
    }
}