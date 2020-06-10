#pragma once
#include "Entity.h"
#include "mylog.h"

namespace jsonrpcpp
{
class Id : public Entity
{
public:
    enum class value_t :uint8_t
    {
        null,
        string,
        integer
    };

    Id()
     : Entity(entity_t::id),
       m_type(value_t::null),
       m_int_id(0),
       m_string_id("")
    {

    }
    Id(int id)
     : Entity(entity_t::id),
       m_type(value_t::integer),
       m_int_id(id),
       m_string_id("")
    {
    }
    Id(const char *id)
     : Entity(entity_t::id),
       m_type(value_t::string),
       m_int_id(0),
       m_string_id(id)
    {

    }
    Id(const std::string& id)
     : Id(id.c_str())
    {

    }
    Id(const Json& json_id): Id()
    {
        Id::parse_json(json_id);
    }

    void parse_json(const Json& json) override
    {
        if(json.is_null()) {
            m_type = value_t::null;
        } else if(json.is_number_integer()) {
            m_type = value_t::integer;
            m_int_id = json.get<int>();
        } else if(json.is_string()) {
            m_type = value_t::string;
            m_string_id = json.get<std::string>();
        } else {
            throw std::invalid_argument("id must be integer,string or null");
        }
    }
    Json to_json() const override {
        if((m_type == value_t::null)) {
            return nullptr;
        }
        if(m_type == value_t::string) {
            return m_string_id;
        }
        if(m_type == value_t::integer) {
            return m_int_id;
        }
    }
    friend std::ostream& operator<<(std::ostream& out, const Id& id) {
        out << id.to_json();
        return out;
    }
    const value_t& type() const {
        return m_type;
    }
    int int_id() const {
        return m_int_id;
    }
    const std::string & string_id() const {
        return m_string_id;
    }

protected:
    value_t m_type;
    int m_int_id;
    std::string m_string_id;
};

} // namespace jsonrpcpp
