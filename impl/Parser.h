#pragma once
#include "types.h"
#include <string>
#include "RpcException.h"
#include "ParseErrorException.h"
#include <memory>
#include "Request.h"

namespace jsonrpcpp
{
class Parser
{
public:
    Parser() = default;
    virtual ~Parser() = default;

    void register_request_callback(
        const std::string& request,
        request_callback callback
    )
    {
        if(callback) {
            m_request_callbacks[request] = callback;
        }
    }
    void register_notification_callback(const std::string& notification, notification_callback cb) {
        if(cb) {
            m_notification_callbacks[notification] = cb;
        }
    }

    entity_ptr parse(const std::string& json_str) {
        entity_ptr entity = do_parse(json_str);
        if(entity && entity->is_notification()) {
            notification_ptr notification = std::dynamic_pointer_cast<Notification>(entity);
            if(m_notification_callbacks.find(notification->method()) != m_notification_callbacks.end()) {
                notification_callback cb = m_notification_callbacks[notification->method()];
                if(cb) {
                    cb(notification->params());
                }
            }
        } else if(entity && entity->is_request()) {
            request_ptr request = std::dynamic_pointer_cast<jsonrpcpp::Request>(entity);
            if(m_request_callbacks.find(request->method()) != m_request_callbacks.end()) {
                request_callback cb = m_request_callbacks[request->method()];
                if(cb) {
                    response_ptr response = cb(request->id(), request->params());
                    if(response) {
                        return response;
                    }

                }
            }
        }
        return entity;
    }

    entity_ptr do_parse(const std::string& json_str) {
        try {
            return do_parse_json(Json::parse(json_str));
        } catch(const RpcException &e) {
            throw;
        } catch(const std::exception& e) {
            throw ParseErrorException(e.what());
        }
        return nullptr;
    }
    entity_ptr do_parse_json(const Json& json) {
        try {
            if(is_request(json)) {
                return std::make_shared<Request>(json);
            }
            if(is_notification(json)) {
                return std::make_shared<Notification>(json);
            }
            if(is_response(json)) {
                return std::make_shared<Response>(json);
            }

        } catch(const RpcException& e) {
            throw;
        } catch(const std::exception& e) {
            throw RpcException(e.what());
        }
        return nullptr;
    }

    bool is_request(const Json& json) {
        return (
            (json.count("method")!=0)
            && (json.count("id")!=0)
        );
    }
    bool is_notification(const Json& json) {
        return (
            (json.count("method")!=0)
            && (json.count("id") == 0)
        );
    }
    bool is_response(const std::string& json_str) {
        try {
            return is_response(Json::parse(json_str));
        } catch(const std::exception& e) {
            return false;
        }
    }
    bool is_response(const Json& json) {
        return (
            (
                (json.count("result")!=0)
                || (json.count("error")!=0)
            )
            && json.count("id") != 0
        );
    }
private:
    std::map<std::string, notification_callback> m_notification_callbacks;
    std::map<std::string, request_callback> m_request_callbacks;
};
} // namespace jsonrpcpp
