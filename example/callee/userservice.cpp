//
// Created by lmy on 2023/3/4.
//

#include "rpcserver.h"
#include "user.pb.h"

static ultra::Logger::ptr g_logger = ULTRA_LOG_ROOT();

class UserService : public fixbug::UserServiceRpc // 使用在rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(std::string name, std::string pwd) {
        ULTRA_LOG_INFO(g_logger) << "doing local service: Login";
        ULTRA_LOG_INFO(g_logger) << "name:" << name << " pwd:" << pwd;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd) {
        ULTRA_LOG_INFO(g_logger) << "doing local service: Register";
        ULTRA_LOG_INFO(g_logger) << "id:" << id << "name:" << name << " pwd:" << pwd;
        return true;
    }

    /*
    重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    1. caller   ===>   Login(LoginRequest)  => muduo =>   callee
    2. callee   ===>    Login(LoginRequest)  => 交到下面重写的这个Login方法上了
    */
    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done) {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入  包括错误码、错误消息、返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_sucess(login_result);

        // 执行回调操作   执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }

    void Register(::google::protobuf::RpcController *controller,
                  const ::fixbug::RegisterRequest *request,
                  ::fixbug::RegisterResponse *response,
                  ::google::protobuf::Closure *done) {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_sucess(ret);

        done->Run();
    }
};


void run() {


    RpcServer::ptr rpcServer(new RpcServer());

    rpcServer->NotifyService(new UserService);

    rpcServer->Run();
}


int main(int argc, char *argv[]) {

    ultra::IOManager iom(1, true, "main");
    iom.schedule(&run);
    return 0;
}