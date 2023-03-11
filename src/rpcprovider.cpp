//
// Created by lmy on 2023/3/2.
//

#include "rpcprovider.h"
#include "rpcserver.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();

/*static ultra::ConfigVar<uint16_t>::ptr rpc_server_port = ultra::Config::Lookup<uint16_t>("rpcServer.port", 12345,
                                                                                         "rpc server port");

static ultra::ConfigVar<std::string>::ptr rpc_server_ip = ultra::Config::Lookup<std::string>("rpcServer.ip",
                                                                                             std::string("127.0.0.1"),
                                                                                             "rpc server ip");*/

void RpcProvider::NotifyService(google::protobuf::Service *service) {

    ServiceInfo service_info;

    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();

    //获取服务名字
    std::string service_name = pserviceDesc->name();

    //获取方法的数量
    int methodCnt = pserviceDesc->method_count();

    ULTRA_LOG_INFO(g_logger) << service_name;

    for (int i = 0; i < methodCnt; i++) {
        //获取了服务对象指定下标的服务方法的描述
        const google::protobuf::MethodDescriptor *pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        ULTRA_LOG_INFO(g_logger) << method_name;
    }

    service_info.m_service = service;

    m_serviceMap.insert({service_name, service_info});


}

void RpcProvider::Run() {

    std::string ip = rpc_server_ip->getValue();
    uint16_t port = rpc_server_port->getValue();

    std::string ip_port = ip + ":" + std::to_string(port);

    ultra::TcpServer::ptr rpcServer(new RpcServer);

    auto rpcAddress = ultra::Address::LookupAny(ip_port);

    ULTRA_ASSERT(rpcAddress);

    while (!rpcServer->bind(rpcAddress)) {
        sleep(2);
    }

    ULTRA_LOG_INFO(g_logger) << "bind success " << rpcServer->getName();


    rpcServer->start();
}

