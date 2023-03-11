//
// Created by lmy on 2023/3/6.
//

#include "rpcsession.h"

#include <utility>


RpcSession::RpcSession(ultra::Socket::ptr socket, bool ower) : SocketStream(std::move(socket), ower) {

}

int RpcSession::sendProtobuf(google::protobuf::Message *response) {

    ultra::ByteArray::ptr recvByte;


}

google::protobuf::Message *RpcSession::recvProtobuf() {

}
