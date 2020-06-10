#pragma once

#include "RpcEntityException.h"
#include "Error.h"

namespace jsonrpcpp
{
class ParseErrorException : public RpcEntityException
{
public:
    ParseErrorException(const Error& error)
     : RpcEntityException(error)
    {

    }
    ParseErrorException(const std::string& data)
     : ParseErrorException(Error("Parse error", -32700, data))
    {

    }
    Json to_json() const override {
        Json response = {
            {"jsonrpc", "2.0"},
            {"error", m_error.to_json()},
            {"id", nullptr}
        };
        return response;
    }

};

void Entity::parse(const char * json_str) {
    try {
        parse_json(Json::parse(json_str));
    } catch(const RpcException& e) {
        throw;
    } catch(const std::exception& e) {
        throw ParseErrorException(e.what());
    }
}
} // namespace jsonrpcpp
