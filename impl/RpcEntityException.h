#pragma once
#include "RpcException.h"
#include "Entity.h"
#include "Error.h"

namespace jsonrpcpp
{
class RpcEntityException: public RpcException, public Entity
{
public:
    RpcEntityException(const Error& error)
    : RpcException(error.message()),
      Entity(entity_t::exception),
      m_error(error)
    {

    }
    Json to_json() const override = 0;
    const Error& error() const {
        return m_error;
    }
    
protected:
    void parse_json(const Json& json) override {

    }
    Error m_error;

};

} // namespace jsonrpcpp
