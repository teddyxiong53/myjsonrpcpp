#pragma once

#include "Entity.h"
#include "Parser.h"

namespace jsonrpcpp
{
class Batch: public Entity
{
public:
    std::vector<entity_ptr> m_entities;
    Batch(const Json& json = nullptr)
    : Entity(entity_t::batch)
    {
        if(json != nullptr) {
            parse_json(json);
        }
    }
    Json to_json() const override {
        Json result ;
        for(const auto &j : m_entities) {
            result.push_back(j->to_json());
        }
        return result;
    }
    void parse_json(const Json& json) override ;

    template <typename T>
    void add(const T& entity) {
        m_entities.push_back(std::make_shared<T>(entity));
    }
    void add_ptr(const entity_ptr& entity) {
        m_entities.push_back(entity);
    }
};
} // namespace jsonrpcpp

