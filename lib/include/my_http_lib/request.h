#include <utility>

#pragma once

namespace my_http_parser {
    class Request
    {
    public:
        Request() = default;
        Request(Method method, std::string  path, std::string  body) :
            m_method(method),
            m_path(std::move(path)),
            m_body(std::move(body)) {}

        [[nodiscard]] Method GetMethod() const { return m_method; }
        [[nodiscard]] std::string GetPath() const { return m_path; }
        [[nodiscard]] std::string GetBody() const { return m_body; }

        void SetMethod(Method method) { m_method = method; }
        void SetPath(const std::string& path) { m_path = path; }
        void SetBody(const std::string& body) { m_body = body; }
    private:
        Method      m_method;
        std::string m_path;
        std::string m_body;
    };
}