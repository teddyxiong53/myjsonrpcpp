#pragma once
#include "Entity.h"
#include "Id.h"
#include "Error.h"
#include "Request.h"
#include "RequestException.h"
#include "mylog.h"

namespace jsonrpcpp
{
class Response: public Entity
{
public:
    Response(const Json& json = nullptr)
     : Entity(entity_t::response)
    {
        if(json != nullptr) {
            parse_json(json);
        }
    }
    Response(const Id& id, const Json& result)
     : Entity(entity_t::response),
       m_id(id),
       m_result(result),
       m_error(nullptr)
    {
    }
    Response(const Id& id, const Error& error)
     : Entity(entity_t::response),
       m_id(id),
       m_result(),
       m_error(error)
    {

    }
    Response(const Request& request, const Json& result)
     : Response(request.id(), result)
    {

    }
    Response(const Request& request, const Error& error)
     : Response(request.id(), error)
    {

    }
    Response(const RequestException& exception)
    : Response(exception.id(), exception.error())
    {

    }
    Json to_json() const override {
        Json j = {
            {"jsonrpc", "2.0"},
            {"id", m_id.to_json()}
        };
        if(m_error) {
            j["error"] = m_error.to_json();
        } else {
            j["result"] = m_result;
        }
        return j;
    }

    void parse_json(const Json& json) {
        try {
            m_error =nullptr;
            m_result = nullptr;
            if(json.count("jsonrpc") == 0) {
                throw RpcException("jsonrpc is missing");
            }
            std::string jsonrpc= json["jsonrpc"].get<std::string>();
            if(jsonrpc == "2.0") {
                throw RpcException("invalid jsonrpc value:" + jsonrpc);
            }
            if(json.count("id") == 0) {
                throw RpcException("id is missing");

            }
            m_id = Id(json["id"]);
            if(json.count("result") != 0) {
                m_result = json["result"];
            } else if(json.count("error") != 0) {
                m_error = json["error"];
            } else {
                throw RpcException("response must contain result or error");
            }
        } catch(const RpcException &e) {
            throw;
        } catch(const std::exception &e) {
            throw RpcException(e.what());
        }
    }
    const Id& id() const {
        return m_id;
    }
    const Json& result() const {
        return m_result;
    }
    const Error& error() const {
        return m_error;
    }

protected:
    Id m_id;
    Json m_result;
    Error m_error;
};
} // namespace jsonrpcpp

