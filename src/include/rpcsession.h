//
// Created by lmy on 2023/3/6.
//

#ifndef UPRPC_RPCSESSION_H
#define UPRPC_RPCSESSION_H

#include <google/protobuf/message.h>
#include "ultra.h"


class RpcSession : public ultra::SocketStream {

public:

    typedef std::shared_ptr<RpcSession> ptr;

    explicit RpcSession(ultra::Socket::ptr socket, bool ower = true);


    int sendProtobuf(google::protobuf::Message *response);


    google::protobuf::Message *recvProtobuf();


};


#endif //UPRPC_RPCSESSION_H
