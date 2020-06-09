#pragma once

#include "RequestException.h"


namespace jsonrpcpp
{
    class Request;
    class InternalErrorException : public RequestException
    {
    public:
        InternalErrorException(const Id &requestId = Id())
            : RequestException(Error("internal error", -32603), requestId)
        {
        }
        InternalErrorException(const Request &request);
        InternalErrorException(const char *data, const Id &requestId = Id())
            : RequestException(Error("internal error", -32603, data), requestId)
        {
        }
        InternalErrorException(const std::string data, const Id& requestId)
         : InternalErrorException(data.c_str(), requestId)
        {

        }
    };
} // namespace jsonrpcpp
