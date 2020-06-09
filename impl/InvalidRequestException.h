#pragma once
#include "RequestException.h"
#include "Error.h"


namespace jsonrpcpp
{
class Request;
class InvalidRequestException: public RequestException
{
public:
    InvalidRequestException(const Id& requestId = Id())
     : RequestException(Error("Invalid request", -32600), requestId)
    {

    }
    InvalidRequestException(const Request& request);
    InvalidRequestException(const std::string& data, const Id& requestId)
    : InvalidRequestException(data.c_str(), requestId)
    {

    }
    InvalidRequestException(const char *data, const Id& requestId)
     : RequestException(Error("invalid request", -32600, data), requestId)
    {

    }
};
} // namespace jsonrpcpp
