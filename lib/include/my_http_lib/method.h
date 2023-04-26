#pragma once

#include <string>

namespace my_http_parser
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

    /**
     * Parse the method from a string coming from the raw request
     * @param method The raw method
     * @param size The size of the raw method
     * @return The method parsed
     */
    static Method ParseMethod(const char* method, size_t  size);

    /**
     * Parse the method from a string coming from the raw request
     * @param method The raw method
     * @param size The size of the raw method
     * @return The method parsed
     */
    static Method ParseMethod(const std::string& method);
}