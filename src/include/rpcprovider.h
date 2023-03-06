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


};


#endif //UPRPC_RPCPROVIDER_H
