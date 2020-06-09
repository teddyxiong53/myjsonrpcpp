#pragma once

#include "NullableEntity.h"

namespace jsonrpcpp
{

class Parameter: public NullableEntity
{
public:
    enum class value_t : uint8_t {
        null,
        array,
        map
    };

    Parameter(std::nullptr_t)
     : NullableEntity(entity_t::id, nullptr),
       m_type(value_t::null)
    {

    }
    Parameter(const Json& json = nullptr)
     : NullableEntity(entity_t::id),
       m_type(value_t::null)
    {

    }
    Parameter(const std::string& k1, const Json& v1,
            const std::string& k2, const Json& v2,
            const std::string& k3, const Json& v3,
            const std::string& k4, const Json& v4
    )
     : NullableEntity(entity_t::id), m_type(value_t::map)
    {
        param_map[k1] = v1;
        if(!k2.empty()) {
            param_map[k2] = v2;
        }
        if(!k3.empty()) {
            param_map[k3] = v3;
        }
        if(!k4.empty()) {
            param_map[k4] = v4;
        }
    }

    Json to_json() const override {
        if(m_type == value_t::array) {
            return param_array;
        }
        if(m_type == value_t::map) {
            return param_map;
        }
        return nullptr;
    }

    void parse_json(const Json& json) {
        if(json.is_array()) {
            param_array = json.get<std::vector<Json>>();
            param_map.clear();
            m_type = value_t::array;
        } else {
            param_map = json.get<std::map<std::string, Json>>();
            param_array.clear();
            m_type = value_t::map;
        }
    }
    bool is_array() const {
        return m_type == value_t::array;
    }
    bool is_map() const {
        return m_type == value_t::map;
    }
    bool is_null() const {
        return m_isNull;
    }
    bool has(const std::string& key) const {
        if(m_type != value_t::map) {
            return false;
        }
        return (param_map.find(key) != param_map.end());
    }
    Json get(const std::string& key) const {
        return param_map.at(key);
    }
    bool has(size_t idx) const {
        if(m_type != value_t::array) {
            return false;
        }
        return (param_array.size() > idx);
    }
    Json get(size_t idx ) const {
        return param_array.at(idx);
    }

    template <typename T>
    T get(const std::string& key) const {
        return get(key).get<T>();
    }
    template <typename T>
    T get(size_t idx) const {
        return get(idx).get<T>();
    }
    template <typename T>
    T get(const std::string& key, const T& default_value) const {
        if(!has(key)) {
            return default_value;
        }
        return get<T>(key);
    }
    template <typename T>
    T get(size_t idx, const T& default_value) const {
        if(!has(idx)) {
            return default_value;
        }
        return get<T>(idx);
    }

    value_t m_type;
    std::vector<Json> param_array;
    std::map<std::string, Json> param_map;

};

} // namespace jsonrpcpp
