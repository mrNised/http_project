#include <my_http_lib/method.h>
#include <iostream>

namespace my_http_lib {

    Method ParseMethod(const char* method, size_t  size)
    {
        //TODO : parse the method from the data sent by llhttp
        return char_to_enum(method);
    }

    Method char_to_enum(const char *msg) {
        Method method;
        auto it = VerbEnumMap.find(msg);
        if (it != VerbEnumMap.end()) {
            method = it->second;
            std::cout << "Good answer" << std::endl;
        } else {
            std::cout << "There was an error while finding the verb" << std::endl;
            method = Method::Unknown;
        }
        return method;
    }

    Method ParseMethod(const std::string& method)
    {
        return ParseMethod(method.c_str(), method.size());
    }
}