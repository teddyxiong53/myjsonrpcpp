#pragma once

#include "NullableEntity.h"
#include "RpcException.h"

namespace jsonrpcpp
{

class Error : public NullableEntity
{
public:
    Error(const Json& json = nullptr)
     : Error("Internal error", -32603, nullptr)
    {
        if(json != nullptr) {
            parse_json(json);
        }
    }
    Error(const std::string& message, int code, const Json& data=nullptr)
     : NullableEntity(entity_t::error, nullptr),
       m_code(code),
       m_message(message),
       m_data(data)
    {

    }
    Error(std::nullptr_t)
     : NullableEntity(entity_t::error, nullptr),
       m_code(0),
       m_message(""),
       m_data(nullptr)
    {

    }

    Json to_json() const override {
        Json j = {
            {"code", m_code},
            {"message", m_message}
        };
        if(!m_data.is_null()) {
            j["data"] = m_data;
        }
        return j;
    }
    void parse_json(const Json& json)  override {
        try {
            if(json.count("code") == 0) {
                throw RpcException("code is missing");
            }
            m_code = json["code"];
            if(json.count("message") == 0) {
                throw RpcException("message is missing");
            }
            m_message = json["message"].get<std::string>();
            if(json.count("data") != 0) {
                m_data = json["data"];
            } else {
                m_data = nullptr;
            }
        } catch(const RpcException& e) {
            throw ;
        } catch(const std::exception &e) {
            throw RpcException(e.what());
        }
    }
    const int code() const {
        return m_code;
    }
    const std::string& message() const {
        return m_message;
    }
    const Json& data() const {
        return m_data;
    }

protected:
    int m_code;
    std::string m_message;
    Json m_data;
};


} // namespace jsonrpcpp

