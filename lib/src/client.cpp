#include "my_http_lib/client.h"

namespace my_http_lib {
    Client::Client(const std::string sevrerIp, uint16_t serverPort) {
        //1 - Connect the client socket to the server
        //2 - Initialize the llhttp parser using llhttp_settings_t like in the example from https://github.com/nodejs/llhttp README
        //          - Register our callbacks
        //              - static int HandleOnMessageComplete(llhttp_t* m_parser);
        //              - static int HandleStatusParsed(llhttp_t* m_parser);
        //              - static int HandleBodyParsed(llhttp_t* m_parser);
        //         - Once the parser is initialized, set this at the parser data this will allow to get this later in our callbacks
        //              - m_parser->data = this;
    }

    Client::~Client() {
        //1 - Close the socket
        //2 - Cleanup the llhttp parser
        //3 - Delete any pointer
    }

    Response Client::LaunchRequest(const Request &request)
    {
        // 1 - Convert the request object to a char*
        //          - Remember how the HTTP request is done : https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
        //          - You just need basic stuff so no headers. Just method, path, version and body
        //          - Remember that a HTTP request MUST end with /r/n/r/n
        // 2 - Send the new request to the server
        // 3 - Recv until you get an answer
        // 4 - Start the parsing using llhttp and wait for it to finish (HandleOnMessageComplete method will help)
        // 5 - Returns the parsed response

        return m_currentResponse;
    }

    int Client::HandleOnMessageComplete(llhttp_t *m_parser)
    {
        // 1 - Stop the wait in LaunchRequest (use m_parser->data to get back your this pointer)
        return 0;
    }

    int Client::HandleStatusParsed(llhttp_t *m_parser) {
        // 1 - Fill the status in the object response (m_currentResponse) (use m_parser->data to get back your this pointer)
        return 0;
    }

    int Client::HandleBodyParsed(llhttp_t *m_parser) {
        // 1 - Fill the body in the object response (m_currentResponse) (use m_parser->data to get back your this pointer)
        return 0;
    }
}