//
// Created by lmy on 2023/3/4.
//

#include "rpcprovider.h"


void run() {
    RpcProvider myRpcServer;
    myRpcServer.Run();
}


int main(int argc, char *argv[]) {

    ultra::IOManager iom(1, true, "main");
    iom.schedule(&run);
    return 0;
}