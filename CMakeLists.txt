cmake_minimum_required(VERSION 3.0)
project(upRPC)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
# 指定编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -O0 -ggdb -Wall -Werror")

# -rdynamic: 将所有符号都加入到符号表中，便于使用dlopen或者backtrace追踪到符号
# -fPIC: 生成位置无关的代码，便于动态链接
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -fPIC")

# -Wno-unused-function: 不要警告未使用函数
# -Wno-builtin-macro-redefined: 不要警告内置宏重定义，用于重定义内置的__FILE__宏
# -Wno-deprecated: 不要警告过时的特性
# -Wno-deprecated-declarations: 不要警告使用带deprecated属性的变量，类型，函数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")


include_directories(/usr/local/protobuf/include)

include_directories(/home/ubuntu/projects/ultraServer/ultra)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

link_directories(${PROJECT_SOURCE_DIR}/lib)


add_subdirectory(src)
add_subdirectory(example)



set(TEST_LIB_SRC
        test/test.pb.cc)

add_library(test1 SHARED ${TEST_LIB_SRC})



set(LIBS
        /home/ubuntu/projects/ultraServer/lib/libultra.so
        protobuf
        pthread
        dl
        yaml-cpp
        )


ultra_add_executable(test_ultra "test/test_ultra.cpp" test1 "${LIBS}")


add_executable(test_proto test/test_proto.cpp test/test.pb.cc)

target_link_libraries(test_proto /home/ubuntu/projects/ultraServer/lib/libultra.so pthread protobuf dl yaml-cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)