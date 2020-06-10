#include "jsonrpcpp.hpp"
#include <iostream>
#include "mylog.h"

using namespace jsonrpcpp;
using namespace std;

Parser parser;
jsonrpcpp::Response getResponse(jsonrpcpp::request_ptr request)
{
    if(request->method() == "substract") {

    } else if(request->method() == "sum") {
        mylogd("");
        int result = 0;
        for(const auto& summand: request->params().param_array) {
            result += summand.get<int>();
        }
        return jsonrpcpp::Response(*request, result);
    }
}
void test(const std::string& json_str)
{
    try {
        cout << "--> " << json_str << "\n";
        jsonrpcpp::entity_ptr entity = parser.parse(json_str);

        if(entity) {
            mylogd("type_str:%s", entity->type_str().c_str());
            if(entity->is_response()) {
                cout << "<-- " << entity->to_json().dump() << "\n";
            }
            if(entity->is_request()) {
                std::shared_ptr<Request> request = std::dynamic_pointer_cast<jsonrpcpp::Request>(entity);
                jsonrpcpp::Response response = getResponse(request);
                cout << "<-- " << response.to_json().dump() << "\n";
            } else if(entity->is_notification()) {
                jsonrpcpp::notification_ptr notifcation = dynamic_pointer_cast<jsonrpcpp::Notification>(entity);
                cout << "Notifcation: " << notifcation->method() << ", has params: " << !notifcation->params().is_null() << "\n";
            } else if(entity->is_batch()) {
                mylogd("");
                batch_ptr batch = std::dynamic_pointer_cast<Batch>(entity);
                Batch responseBatch;
                mylogd("size:%d", batch->m_entities.size());
                for(const auto & batch_entity: batch->m_entities) {
                    mylogd("entity type:%s", batch_entity->type_str().c_str());
                    if(batch_entity->is_request()) {
                        try {
                            request_ptr request = std::dynamic_pointer_cast<Request>(batch_entity);
                            Response response = getResponse(request);
                            responseBatch.add(response);
                        } catch (const RequestException& e) {
                            responseBatch.add(e);
                        }
                    } else if(batch_entity->is_exception()) {
                        responseBatch.add_ptr(batch_entity);
                    } else if(batch_entity->is_error()) {
                        error_ptr error = std::dynamic_pointer_cast<Error>(batch_entity);
                        responseBatch.add(RequestException(*error));
                    }
                }
                if(!responseBatch.m_entities.empty()) {
                    cout << "<-- " << responseBatch.to_json().dump() << "\n";
                }
            }

        } else {
            mylogd("");
        }
    } catch(const RequestException& e) {
        myloge("%s", e.to_json().dump().c_str());
    } catch(const ParseErrorException& e) {
        myloge("%s", e.to_json().dump().c_str());
    } catch(const RpcException &e) {
        myloge("%s", e.what());
    } catch(...) {
        myloge("unknown error");
    }
}

void test(const Entity& entity)
{
    test(entity.to_json().dump());
}
jsonrpcpp::response_ptr sum(const jsonrpcpp::Id& id, const jsonrpcpp::Parameter& params)
{
    int result = 0;
    for(const auto summand: params.param_array) {
        result += summand.get<int>();
    }
    cout << "request callback, sum , result: " << result << "\n";
    return std::make_shared<jsonrpcpp::Response>(id, result);
}
void foo(const Parameter& params)
{

}

int main(int argc, char const *argv[])
{
    //parser.register_request_callback("sum", sum);
    //parser.register_notification_callback("foo", foo);
    //这个是正确测试。
    //test(R"({"jsonrpc": "2.0", "method": "sum", "params": [1,2,3], "id": 1})");
    //这个测试RequestException，版本号不对。
    //test(R"({"jsonrpc": "1.0", "method": "sum", "params": [1,2,3], "id": 1})");
    //这个测试ParseErrorException，把id去掉一边的引号。
    //这个会触发2个异常。一个是版本号的，一个是解析错误。
    //test(R"({"jsonrpc": "1.0", "method": "sum", "params": [1,2,3], "id: 1})");

    //test(jsonrpcpp::Request(1, "sum", Json({1,2,3})));
    // test(Notification("foo", Json({})));

    // test(R"([])");
    // test(R"([1])");
        test(R"([
		{"jsonrpc": "2.0", "method": "sum", "params": [1,2,4], "id": "1"},
        {"jsonrpc": "2.0", "method": "sum", "params": [1,3,4], "id": "2"}
        ])");

    return 0;
}

