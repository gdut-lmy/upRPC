//
// Created by lmy on 2023/3/2.
//

#ifndef UPRPC_UPRPCAPPLICATION_H
#define UPRPC_UPRPCAPPLICATION_H

//uprpc框架的初始化类

#include "ultra.h"

namespace uprpc {
    class UprpcApplication {
    public:
        UprpcApplication();


    private:

    };

    typedef ultra::Singleton<UprpcApplication> UprpcApp;
}


#endif //UPRPC_UPRPCAPPLICATION_H
