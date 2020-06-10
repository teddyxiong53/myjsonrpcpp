#pragma once
#include "Entity.h"
#include "Parameter.h"
#include "RpcException.h"

namespace jsonrpcpp
{

class Notification: public Entity
{
public:
    Notification(const Json& json = nullptr)
    : Entity(entity_t::notification)
    {

        if(json != nullptr) {
            Notification::parse_json(json);
        }
    }
    Notification(const char *method, const Parameter& params = nullptr)
    : Entity(entity_t::notification),
      m_method(method),
      m_params(params)
    {

    }
    Notification(const std::string& method, const Parameter& params)
    : Notification(method.c_str(), params)
    {

    }

    Json to_json() const override {
        Json json = {
            {"jsonrpc", "2.0"},
            {"method", m_method}
        };
        if(m_params) {
            json["params"] = m_params.to_json();
        }
        return json;
    }
    void parse_json(const Json& json) override {
        try {

            if(json.count("jsonrpc") == 0) {
                throw RpcException("jsonrpc is missing");
            }
            std::string jsonrpc = json["jsonrpc"].get<std::string>();
            if(jsonrpc != "2.0") {
                throw RpcException("invalid jsonrpc value:" + jsonrpc);
            }
            if(json.count("method") == 0) {
                throw RpcException("method is missing");
            }

            if(!json["method"].is_string()) {
                throw RpcException("method must be string");
            }
            m_method = json["method"].get<std::string>();
            if(m_method.empty()) {
                throw RpcException("method must not be emtpy");
            }
            if(json.count("params") != 0) {

                m_params.parse_json(json["params"]);
            } else {
                m_params = nullptr;
            }
        } catch(const RpcException& e) {
            throw;
        } catch(const std::exception& e) {
            throw RpcException(e.what());
        } catch(...) {
            mylogd("");
        }
    }
    const std::string& method() const {
        return m_method;
    }
    const Parameter& params() const {
        return m_params;
    }

protected:
    std::string m_method;
    Parameter m_params;
};
} // namespace jsonrpcpp

