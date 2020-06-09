#pragma once
#include <memory>
#include <utility>
#include <functional>

namespace jsonrpcpp
{
class Entity;
class Request;
class Response;
class Parameter;
class Notification;
class Error;
class Batch;
class Id;

using entity_ptr = std::shared_ptr<Entity>;
using request_ptr = std::shared_ptr<Request>;
using notification_ptr = std::shared_ptr<Notification>;
using parameter_ptr = std::shared_ptr<Parameter>;
using response_ptr = std::shared_ptr<Response>;
using error_ptr = std::shared_ptr<Error>;
using batch_ptr = std::shared_ptr<Batch>;


typedef std::function<void (const Parameter& params)> notification_callback;
typedef std::function<response_ptr(const Id& id, const  Parameter& params)> request_callback;

} // namespace jsonrpcpp



