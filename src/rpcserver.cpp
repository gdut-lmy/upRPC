//
// Created by lmy on 2023/3/4.
//


#include "rpcserver.h"
#include "rpcheader.pb.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();


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
}

void RpcServer::Run() {


}
