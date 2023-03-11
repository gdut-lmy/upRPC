//
// Created by lmy on 2023/3/4.
//

#ifndef UPRPC_RPCSERVER_H
#define UPRPC_RPCSERVER_H

#include "ultra.h"
#include <google/protobuf/service.h>
#include "zookeeperutil.h"


class RpcServer : public ultra::TcpServer {
public:

    typedef std::shared_ptr<RpcServer> ptr;

    RpcServer(ultra::IOManager *worker = ultra::IOManager::GetThis(),
              ultra::IOManager *accept_worker = ultra::IOManager::GetThis());

    // 这里是框架提供给外部使用的，可以发布rpc方法的函数接口

    void handleClient(ultra::Socket::ptr client) override;


    void setName(const std::string &v) override;

    //框架提供给外部使用，可以用来发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service); //多态，使用Service基类来指向传入的对象

    bool start() override;


private:

    //service服务类型消息
    struct ServiceInfo {
        google::protobuf::Service *m_service;//保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;//保存服务方法
    };
    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    //Closure的回调操作
    void sendRpcResponse(ultra::Socket::ptr &client, google::protobuf::Message *response);


private:
};


#endif //UPRPC_RPCSERVER_H
