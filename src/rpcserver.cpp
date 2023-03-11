//
// Created by lmy on 2023/3/4.
//


#include "rpcserver.h"
#include "rpcheader.pb.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();


static ultra::ConfigVar<uint16_t>::ptr rpc_server_port = ultra::Config::Lookup<uint16_t>("rpcserver.port", 8880,
                                                                                         "rpc server port");

static ultra::ConfigVar<std::string>::ptr rpc_server_ip = ultra::Config::Lookup<std::string>("rpcserver.ip",
                                                                                             std::string("127.0.0.1"),
                                                                                             "rpc server ip");


RpcServer::RpcServer(ultra::IOManager *worker, ultra::IOManager *accept_worker) : TcpServer(worker, accept_worker) {

    RpcServer::setName("rpcServer");

}


void RpcServer::handleClient(ultra::Socket::ptr client) {


    std::string recv_buf;
    recv_buf.resize(4096);
    client->recv(&recv_buf[0], recv_buf.length());

    uint32_t header_size = 0;

    recv_buf.copy((char *) &header_size, 4, 0);

    std::string rpc_header_str = recv_buf.substr(4, header_size);
    uprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str)) {
        // 数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        ULTRA_LOG_INFO(g_logger) << "rpc_header_str: " << rpc_header_str << "parse error! ";
        return;
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    auto it = m_serviceMap.find(service_name);

    if (it == m_serviceMap.end()) {
        ULTRA_LOG_INFO(g_logger) << service_name << " is not exist!";
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        ULTRA_LOG_INFO(g_logger) << service_name << ":" << method_name << " is not exist!";
        return;
    }

    //获取service对象
    google::protobuf::Service *service = it->second.m_service;
    const google::protobuf::MethodDescriptor *method = mit->second;

    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str)) {
        ULTRA_LOG_INFO(g_logger) << "request parse error , content:" << args_str;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();


    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcServer, ultra::Socket::ptr &, google::protobuf::Message *>(
            this, &RpcServer::sendRpcResponse, client, response);

    service->CallMethod(method, nullptr, request, response, done);

}

void RpcServer::setName(const std::string &v) {
    TcpServer::setName(v);
}


void RpcServer::sendRpcResponse(ultra::Socket::ptr &client, google::protobuf::Message *response) {

    std::string response_str;
    if (response->SerializeToString(&response_str)) {
        client->send(&response_str[0], response_str.length());
    } else {
        ULTRA_LOG_INFO(g_logger) << "serialize response_str error!";
    }

    client->close();

}

void RpcServer::NotifyService(google::protobuf::Service *service) {
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

    // 把当前rpc节点上要发布的服务全部注册到zk上面，让rpc client可以从zk上发现服务
    // session timeout   30s     zkclient 网络I/O线程  1/3 * timeout 时间发送ping消息
    ZkClient m_zkCli;
    m_zkCli.Start();
    // service_name为永久性节点    method_name为临时性节点
    for (auto &sp: m_serviceMap) {
        // /service_name   /UserServiceRpc
        std::string service_path = "/" + sp.first;
        m_zkCli.Create(service_path.c_str(), nullptr, 0);
        for (auto &mp: sp.second.m_methodMap) {
            // /service_name/method_name   /UserServiceRpc/Login 存储当前这个rpc服务节点主机的ip和port
            std::string method_path = service_path + "/" + mp.first;
            char method_path_data[128] = {0};
            sprintf(method_path_data, "%s:%d", rpc_server_ip->getValue().c_str(), rpc_server_port->getValue());
            // ZOO_EPHEMERAL表示znode是一个临时性节点
            m_zkCli.Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
        }
    }


}

bool RpcServer::start() {

    std::string ip = rpc_server_ip->getValue();
    uint16_t port = rpc_server_port->getValue();

    std::string ip_port = ip + ":" + std::to_string(port);


    auto rpcAddress = ultra::Address::LookupAny(ip_port);

    ULTRA_ASSERT(rpcAddress);

    while (!this->bind(rpcAddress)) {
        sleep(2);
    }

    ULTRA_LOG_INFO(g_logger) << "bind success " << this->getName();


    return TcpServer::start();
}
