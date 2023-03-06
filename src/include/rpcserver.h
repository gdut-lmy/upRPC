//
// Created by lmy on 2023/3/4.
//

#ifndef UPRPC_RPCSERVER_H
#define UPRPC_RPCSERVER_H

#include "ultra.h"
#include <google/protobuf/service.h>


class RpcServer : public ultra::TcpServer {
public:

    explicit RpcServer(ultra::IOManager *worker = ultra::IOManager::GetThis(),
              ultra::IOManager *accept_worker = ultra::IOManager::GetThis());

    // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口

    void handleClient(ultra::Socket::ptr client) override;

    void setName(const std::string &v) override;


};


#endif //UPRPC_RPCSERVER_H
