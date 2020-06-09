#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace jsonrpcpp
{
class RpcException: public std::exception
{
public:
    RpcException(const char *text)
     : m_error(text)
    {

    }
    RpcException(const std::string& text)
     : RpcException(text.c_str())
    {

    }
    const char *what() const noexcept override{
        return m_error.what();
    }

protected:
    std::runtime_error m_error;
};

} // namespace jsonrpcpp

