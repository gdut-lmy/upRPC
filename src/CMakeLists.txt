set(SRC_LIST
        uprpcapplication.cpp
        rpcprovider.cpp
        rpcserver.cpp
        rpcheader.pb.cc
        rpcchannel.cpp)

add_library(uprpc ${SRC_LIST})

target_link_libraries(uprpc ultra pthread zookeeper_mt)