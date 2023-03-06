set(SRC_LIST calluserservice.cpp ../user.pb.cc)

add_executable(caller ${SRC_LIST})
target_link_libraries(caller uprpc ultra yaml-cpp pthread  dl protobuf)