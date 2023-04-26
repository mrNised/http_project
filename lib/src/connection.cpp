#include "my_http_lib/connection.h"

namespace my_http_parser
{
    Connection::Connection(SOCKET clientSocket, Connection::ConnectionRequestHandler RequestHandler,
                           Connection::ConnectionClosedHandler CloseHandler):
                           m_clientSocket(clientSocket),
                           m_requestHandler(RequestHandler),
                           m_closeHandler(CloseHandler)

    {
        // 1 - Start the recv thread
        //2 - Initialize the llhttp parser using llhttp_settings_t like in the example from https://github.com/nodejs/llhttp README
        //                  - Register our callbacks
        //                      - static int HandleOnMessageComplete(llhttp_t* m_parser);
        //                      - static int HandlePathParsed(llhttp_t* m_parser);
        //                      - static int HandleBodyParsed(llhttp_t* m_parser);
        //                      - static int HandleMethodParsed(llhttp_t* m_parser);
        //                 - Once the parser is initialized, set this at the parser data this will allow to get this later in our callbacks
        //                      - m_parser->data = this;
    }

    Connection::~Connection()
    {
        //1 - Kill the thread
        //2 - Close the socket
        //3 - Cleanup the llhttp parser
        //4 - Delete any pointer
    }

    void Connection::Close()
    {
        //1 - Shutdown and close the socket
    }

    int Connection::HandleOnMessageComplete(llhttp_t *m_parser)
    {
        // 1 - Stop the wait in ThreadUpdate (use m_parser->data to get back your this pointer)
        return 0;
    }

    int Connection::HandlePathParsed(llhttp_t *m_parser)
    {
        // 1 - Fill the path in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        return 0;
    }

    int Connection::HandleMethodParsed(llhttp_t *m_parser)
    {
        // 1 - Fill the method in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        return 0;
    }

    int Connection::HandleBodyParsed(llhttp_t *m_parser)
    {
        // 1 - Fill the body in the object request (m_currentRequest) (use m_parser->data to get back your this pointer)
        return 0;
    }

    void Connection::ThreadUpdate()
    {
        // 1 - Until the connection is closed :
        //          - Recv
        //          - Parse the request from the client
        //          - Wait for the request to be parsed
        //          - Call m_requestHandler with the parsed request and a response
        //          - Send the response to the client :
        //              - Remember how the HTTP message is done : https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
        //          - If RECV fail consider the client closed so call m_closeHandler
    }
}