#include <iostream>

#include <fmt/format.h>
#include <llhttp.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "my_http_lib/http_lib.h"


namespace my_http_lib
{
    static int handle_on_message_complete(llhttp_t*){

    }

    void test()
    {

        std::ifstream f("example.json");
        json data = json::parse(f);

        json ex1 = json::parse(R"(
  {
    "pi": 3.141,
    "happy": true
  }
)");

        //-------------------------------------------------------------------------------------------------------

        llhttp_t parser;
        llhttp_settings_t settings;

/* Initialize user callbacks and settings */
        llhttp_settings_init(&settings);

/* Set user callback */
        settings.on_message_complete = handle_on_message_complete;

/* Initialize the parser in HTTP_BOTH mode, meaning that it will select between
 * HTTP_REQUEST and HTTP_RESPONSE parsing automatically while reading the first
 * input.
 */
        llhttp_init(&parser, HTTP_BOTH, &settings);

/* Parse request! */
        const char* request = "GET / HTTP/1.1\r\n\r\n";
        int request_len = strlen(request);

        enum llhttp_errno err = llhttp_execute(&parser, request, request_len);
        if (err == HPE_OK) {
            /* Successfully parsed! */
        } else {
            fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err),
                    parser.reason);
        }

        //-------------------------------------------------------------------------------------------------------

        std::cout << fmt::format("Hello World from {}", "toto")  << std::endl;
    }
}