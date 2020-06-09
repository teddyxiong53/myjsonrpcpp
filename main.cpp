#include "jsonrpcpp.hpp"

using namespace jsonrpcpp;

int main(int argc, char const *argv[])
{
    Request req(Id(1), std::string("test"), Parameter());

    return 0;
}

