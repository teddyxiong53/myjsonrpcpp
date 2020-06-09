#pragma once

#include "RpcEntityException.h"
#include "Error.h"
#include "Id.h"

namespace jsonrpcpp
{
class RequestException: public RpcEntityException
{
public:
    RequestException(const Error& error, const Id& requestId = Id())
     : RpcEntityException(error),
       m_id(requestId)
    {

    }
    const Id& id() const {
        return m_id;
    }
    Json to_json() const override {
        Json response = {
            {"jsonrpc", "2.0"},
            {"error", m_error.to_json()},
            {"id", m_id.to_json()}
        };
        return response;
    }
protected:
    Id m_id;
};
} // namespace jsonrpcpp
