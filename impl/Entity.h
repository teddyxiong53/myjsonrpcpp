#ifndef _IMPL_ENTITY_H_
#define _IMPL_ENTITY_H_

namespace jsonrpcpp
{

class Entity {//是值语义。
public:
    enum class entity_t : uint8_t {
        unknown,
        exception,
        id,
        error,
        response,
        request,
        notification,
        batch
    };

    Entity(entity_t type) : m_entity(type)
    {

    }
    virtual ~Entity() = default;
    Entity(const Entity& ) = default;//允许拷贝构造
    Entity& operator=(const Entity&) = default;

    bool is_exception() const {
        return m_entity == entity_t::exception;
    }
    bool is_id() const {
        return m_entity == entity_t::id;
    }
    bool is_error() const {
        return m_entity == entity_t::error;
    }
    bool is_response() const {
        return m_entity == entity_t::response;
    }
    bool is_request() const {
        return m_entity == entity_t::request;
    }
    bool is_notification() const {
        return m_entity == entity_t::notification;
    }
    bool is_batch() const {
        return m_entity == entity_t::batch;
    }
    virtual std::string type_str() const {
        switch (m_entity  )
        {
        case entity_t::unknown:
            return "unknown";
        case entity_t::id:
            return "id";
        case entity_t::exception:
            return "exception";
        case entity_t::error:
            return "error";
        case entity_t::response:
            return "response";
        case entity_t::request:
            return "request";
        case entity_t::notification:
            return "notification";
        case entity_t::batch:
            return "batch";

        default:
            return "unknown";
        }
    }
    virtual Json to_json() const = 0;
    virtual void parse_json(const Json& json) = 0;

    virtual void parse(const std::string& json_str);
    virtual void parse(const char * json_str);

protected:
    entity_t m_entity;
};



} // namespace jsonrpcpp

#endif //_IMPL_ENTITY_H_
