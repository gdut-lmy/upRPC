set(SRC_LIST
        uprpcapplication.cpp
        rpcprovider.cpp)

add_library(uprpc ${SRC_LIST})

target_link_libraries(uprpc ultra pthread zookeeper_mt)