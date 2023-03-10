set(SRC_LIST
        uprpcapplication.cpp
        rpcserver.cpp
        rpcheader.pb.cc
        rpcchannel.cpp
        zookeeperutil.cpp)

add_library(uprpc ${SRC_LIST})

target_link_libraries(uprpc ultra pthread zookeeper_mt)