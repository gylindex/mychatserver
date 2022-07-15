# mychatserver
基于muduo库实现了工作在nginx tcp负载均衡环境中的集群聊天服务器和客户端，采用redis消息队列进行集群，mysql数据库存储
编译方法
cd build
rm -rf *
cmake ..
make

运行时需要参数 ip  port
运行前需要配置nginx负载均衡，客户端只需要访问nginx的ip和端口号
启动redis mysql nginx服务
