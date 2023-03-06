


#include <iostream>
#include <user.pb.h>
#include "rpcchannel.h"
#include "ultra.h"

int main(int argc, char **argv) {


    fixbug::UserServiceRpc_Stub stub(new upRpcChannel());

    //客户端请求封装
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    //客户端接受到的响应
    fixbug::LoginResponse response;
    stub.Login(nullptr, &request, &response, nullptr);

    if (0 == response.result().errcode()) {
        std::cout << "rpc login response success:" << response.sucess() << std::endl;
    } else {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }

    return 0;
}