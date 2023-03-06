//
// Created by lmy on 2023/3/2.
//

#include "rpcprovider.h"
#include "rpcserver.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();

static ultra::ConfigVar<uint16_t>::ptr rpc_server_port = ultra::Config::Lookup<uint16_t>("rpc.server.port", 12345,
                                                                                         "rpc server port");

static ultra::ConfigVar<std::string>::ptr rpc_server_ip = ultra::Config::Lookup<std::string>("rpc.server.ip",
                                                                                             std::string("127.0.0.1"),
                                                                                             "rpc server ip");

void RpcProvider::NotifyService(google::protobuf::Service *service) {

}

void RpcProvider::Run() {

    std::string ip = rpc_server_ip->getValue();
    uint16_t port = rpc_server_port->getValue();

    std::string ip_port = ip + ":"+std::to_string(port);

    ultra::TcpServer::ptr rpcServer(new RpcServer);

    auto rpcAddress = ultra::Address::LookupAny(ip_port);

    ULTRA_ASSERT(rpcAddress);

    while (!rpcServer->bind(rpcAddress)) {
        sleep(2);
    }

    ULTRA_LOG_INFO(g_logger) << "bind success " << rpcServer->getName();


    rpcServer->start();
}
