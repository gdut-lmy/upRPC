//
// Created by lmy on 2023/3/6.
//

#ifndef UPRPC_RPCCHANNEL_H
#define UPRPC_RPCCHANNEL_H

#include <google/protobuf/service.h>

class rpcChannel : public google::protobuf::RpcChannel {
public:
    void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller, const google::protobuf::Message *request,
                    google::protobuf::Message *response, google::protobuf::Closure *done);
};

#endif //UPRPC_RPCCHANNEL_H
