//
// Created by lmy on 2023/3/2.
//

#ifndef UPRPC_RPCPROVIDER_H
#define UPRPC_RPCPROVIDER_H

#include "google/protobuf/service.h"
#include <ultra.h>
#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <unordered_map>


class RpcProvider {
public:


    //框架提供给外部使用，可以用来发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service); //多态，使用Service基类来指向传入的对象

    void Run();

private:

    //service服务类型消息
    struct ServiceInfo {
        google::protobuf::Service *m_service;//保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;//保存服务方法
    };
    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;



};


#endif //UPRPC_RPCPROVIDER_H
