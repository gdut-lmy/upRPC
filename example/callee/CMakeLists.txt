set(SRC_LIST userservice.cpp)
#set(SRC_LIST friendservice.cc ../friend.pb.cc)

add_executable(provider ${SRC_LIST})
target_link_libraries(provider uprpc ultra yaml-cpp pthread  dl  protobuf)