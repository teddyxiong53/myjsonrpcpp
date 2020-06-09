#pragma once

#include "Entity.h"
#include "Parameter.h"
#include "Id.h"
#include "InvalidRequestException.h"
#include "InternalErrorException.h"

namespace jsonrpcpp
{
class Request: public Entity
{
public:
    Request(const Json& json = nullptr)
     : Entity(entity_t::request),
       m_method(""),
       m_id()
    {
        if(json != nullptr) {
            Request::parse_json(json);
        }
    }
    Request(const Id& id, const std::string& method, const Parameter& params)
     : Entity(entity_t::request),
       m_method(method),
       m_params(params),
       m_id(id)
    {

    }
    Json to_json() const override {
        Json json = {
            {"jsonrpc", "2.0"},
            {"method", m_method},
            {"id", m_id.to_json()}
        };
        if(m_params) {
            json["params"]  = m_params.to_json();
        }
        return json;
    }
    void parse_json(const Json& json) override {
        try {
            if(json.count("id") == 0) {
                throw InvalidRequestException("id is missing");
            }
            try {
                m_id = Id(json["id"]);
            } catch(const std::exception &e) {
                throw InvalidRequestException(e.what());
            }
            if(json.count("jsonrpc") == 0) {
                throw InvalidRequestException("jsonrpc is missing", m_id);
            }
            std::string jsonrpc= json["jsonrpc"].get<std::string>();
            if(jsonrpc != "2.0") {
                throw InvalidRequestException("invalid jsonrpc value:" + jsonrpc, m_id);
            }
            if(json.count("method")==0) {
                throw InvalidRequestException("method is missing", m_id);
            }
            if(!json["method"].is_string()) {
                throw InvalidRequestException("method must be string", m_id);
            }
            m_method = json["method"].get<std::string>();
            if(m_method.empty()) {
                throw InvalidRequestException("method must not be empty", m_id);
            }
            if(json.count("params") != 0) {
                m_params.parse_json(json["params"]);
            } else {
                m_params = nullptr;
            }
        } catch(const RequestException &e) {
            throw;
        } catch(const std::exception &e) {
            throw InternalErrorException(e.what(), m_id);
        }
    }
    const std::string& method() const {
        return m_method;
    }
    const Parameter& params() const {
        return m_params;
    }
    const Id& id() const {
        return m_id;
    }
protected:
    std::string m_method;
    Parameter m_params;
    Id m_id;
};

InvalidRequestException::InvalidRequestException(const Request& request)
    : InvalidRequestException(request.id())
    {

    }
InternalErrorException::InternalErrorException(const Request &request)
            : InternalErrorException(request.id())
        {
        }
} // namespace jsonrpcpp
