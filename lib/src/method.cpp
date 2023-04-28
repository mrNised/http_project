#include <my_http_lib/method.h>

namespace my_http_lib {
    static Method ParseMethod(const char* method, size_t  size)
    {
        //TODO : parse the method from the data sent by llhttp
        return Method::Unknown;
    }

    static Method ParseMethod(const std::string& method)
    {
        return ParseMethod(method.c_str(), method.size());
    }
}