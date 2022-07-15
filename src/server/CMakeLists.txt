#以下两行通用
#指定最小版本
#cmake_minimum_required(VERSION 3.0)
#设置项目名称
#project(muduo_server)

#配置编译选项
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#配置头文件搜索路径
#include_directories(./include /usr/include/xxx)
#配置库文件搜索路径
#link_directories(./lib /usr/lib/xxx)

#设置需要编译的源文件列表，如果源文件很多，使用第二种方式
#set(SRC_LIST muduo_server.cpp)
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./redis REDIS_LIST)

#设置可执行文件生成到bin目录下去
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#生成可执行文件（欲生成的可执行文件 源文件）
add_executable(muduo_server ${SRC_LIST} ${DB_LIST} ${REDIS_LIST})

#指定可执行文件需要链接的库（可执行程序名 库1 库2 ...）
target_link_libraries(muduo_server muduo_net muduo_base mysqlclient hiredis pthread)