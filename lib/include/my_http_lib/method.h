#pragma once

#include <string>
#include <unordered_map>

namespace my_http_lib
{
    enum class Method
    {
        Get,
        Post,
        Put,
        Delete,
        Option,
        Unknown
    };

    static std::unordered_map<std::string,Method> const VerbEnumMap = {
            {"GET",Method::Get},
            {"POST",Method::Post},
            {"PUT",Method::Put},
            {"DELETE",Method::Delete},
            {"OPTION",Method::Option}
    };

    static std::unordered_map<Method,std::string> const MethodToStringMap = {
            {Method::Get,"GET"},
            {Method::Post,"POST"},
            {Method::Put,"PUT"},
            {Method::Delete,"DELETE"},
            {Method::Option,"OPTION"}
    };

    /**
     * Parse the method from a string coming from the raw request
     * @param method The raw method
     * @param size The size of the raw method
     * @return The method parsed
     */
    Method ParseMethod(const char* method, size_t  size);

    /**
     * Parse the method from a string coming from the raw request
     * @param method The raw method
     * @param size The size of the raw method
     * @return The method parsed
     */
    Method ParseMethod(const std::string& method);

    Method char_to_enum(const char *method);
}