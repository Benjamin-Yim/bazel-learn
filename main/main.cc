
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#ifndef PORT
#define PORT 7000
#endif

#ifndef QUEUE
#define QUEUE 20 //连接请求队列
#endif

int main(int argc, char **argv) {
  /**
   * Socket 服务端编程
   *
   * */
  // 创建 Socket
  // socket(domain, type, protocol)
  // 可以参考： https://www.geeksforgeeks.org/socket-programming-cc/
  int fd = socket(AF_INET, SOCK_STREAM, 0); // 返回一个 Socket 文件描述符号
  if (fd == 0) {
    std::cout << "application Socket source fail" << std::endl;
    return 0;
  }
  /*
   * 设置 Socket 属性
   * */

  /*
   * 绑定 IP 端口
   * */
  struct sockaddr_in server_address;
  // 绑定端口，Htonl 将一个无符号短整型数值转换为网络字节序，即大端模式
  server_address.sin_port = htons(PORT);
  // INADDR_ANY：任意地址类型，也可以指定：inet_addr("127.0.0.1");
  server_address.sin_addr.s_addr = INADDR_ANY;
  // 使用 IP V4 地址
  server_address.sin_family = AF_INET;
  // int bind(int sockfd, const struct sockaddr *addr,  socklen_t addrlen);
  int result =
      bind(fd, (struct sockaddr *)&server_address, sizeof(server_address));
  if (result < 0) {
    std::cout << "bind fail" << std::endl;
    return 0;
  }
  /*
   * 开启监听
   * */
  // int listen(int sockfd, int backlog);
  // 第一个参数是当前 SOcket 文件描述符，第二个参数是 当前可以阻塞的队列大小
  int l = listen(fd, QUEUE);
  if (l < 0) {
    std::cout << "listen fail" << std::endl;
  }
  /*
   * 接收客户端传过来的连接
   * */
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  // accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
  // 成功返回非负描述字，出错返回-1
  int clientfd = accept(fd, (struct sockaddr *)&client_addr, &length);
  // 如果accpet成功，那么其返回值是由内核自动生成的一个全新描述符，代表与所返回客户的TCP连接。
  if (clientfd < 0) {
    std::cout << "accept fail" << std::endl;
    return 0;
  }
  /*
   * 收发数据
   * */
  // 设置缓冲区
  char buffer[1024];
  while (1) {
    // 缓冲区设置为空
    bzero(buffer, sizeof(buffer));
    int len = read(clientfd, buffer, sizeof(buffer) - 1);
    if (len < 0) {
      std::cout << "remote server close" << std::endl;
      break;
    }
    std::cout << "recv message:" << buffer << std::endl;
    send(clientfd, "hello", strlen("hello"), 0);
  }

  /*
   * 关闭 Socket
   * */
  close(clientfd);
  /*
   * 关闭监听
   * */
  close(fd);
  return 0;
}
