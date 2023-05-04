#pragma once

#include <string>

#include <my_http_lib/method.h>

namespace my_http_lib {
    class Response
    {
    public:
        /**
         * Return the HTTP code of the response
         * @return
         */
        [[nodiscard]] uint8_t GetCode() const { return m_code; }

        /**
         * Return the body of the response
         * @return
         */
        [[nodiscard]] const std::string& GetBody() const { return m_body; }

        void SetCode(uint8_t code) { m_code = code; }
        void SetBody(std::string body) { m_body = body; }
    private:
        uint8_t     m_code;
        std::string m_body;
    };
}