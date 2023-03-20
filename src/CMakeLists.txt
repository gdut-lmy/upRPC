set(SRC_LIST
        rpcserver.cpp
        rpcheader.pb.cc
        rpcchannel.cpp
        zookeeperutil.cpp
        route_strategy.cpp)

add_library(uprpc ${SRC_LIST})

target_link_libraries(uprpc ultra pthread zookeeper_mt)