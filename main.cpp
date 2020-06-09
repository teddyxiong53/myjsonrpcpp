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
            if(entity->is_response()) {
                cout << "<-- " << entity->to_json().dump() << "\n";
            }
            if(entity->is_request()) {
                jsonrpcpp::Response response = getResponse(std::dynamic_pointer_cast<jsonrpcpp::Request>(entity));
                cout << "<-- " << response.to_json().dump() << "\n";
            } else if(entity->is_notification()) {
                jsonrpcpp::notification_ptr notifcation = dynamic_pointer_cast<jsonrpcpp::Notification>(entity);
                cout << "Notifcation: " << notifcation->method() << ", has params: " << !notifcation->params().is_null() << "\n";
            }

        }
    } catch(...) {
        mylogd("");
    }
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
int main(int argc, char const *argv[])
{
    parser.register_request_callback("sum", sum);
    test(R"({"jsonrpc": "2.0", "method": "sum", "params": [1,2,3], "id": 1})");
    return 0;
}

