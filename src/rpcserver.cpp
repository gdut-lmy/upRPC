//
// Created by lmy on 2023/3/4.
//


#include "rpcserver.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();


RpcServer::RpcServer(ultra::IOManager *worker, ultra::IOManager *accept_worker) : TcpServer(worker, accept_worker) {

    RpcServer::setName("rpcServer");

}


void RpcServer::handleClient(ultra::Socket::ptr client) {

    ULTRA_LOG_INFO(g_logger) << "new client: " << client->toString();
    static std::string buf;
    buf.resize(4096);
    client->recv(&buf[0], buf.length()); // 这里有读超时，由tcp_server.read_timeout配置项进行配置，默认120秒
    ULTRA_LOG_INFO(g_logger) << "recv: " << buf;
    client->close();

}

void RpcServer::setName(const std::string &v) {
    TcpServer::setName(v);
}


