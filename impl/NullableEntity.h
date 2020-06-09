#pragma once
#include "Entity.h"

namespace jsonrpcpp
{
class NullableEntity: public Entity
{
public:
    NullableEntity(entity_t type)
     : Entity(type),
       m_isNull(false)
    {

    }
    NullableEntity(entity_t type, std::nullptr_t)
     : Entity(type), m_isNull(true)
    {

    }
    virtual explicit operator bool() const {
        return !m_isNull;
    }

protected:
    bool m_isNull;
};

} // namespace jsonrpcpp


