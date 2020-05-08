
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
#include <unistd.h>

#ifndef PORT
#define PORT 7000
#endif

#ifndef QUEUE
#define QUEUE 20 //连接请求队列
#endif

#ifndef CLIENT_INFO_
#define MYPORT 7000
#define BUFFER_SIZE 1024
#endif

int main(int argc, char **argv) {
  fd_set rdfs;

  /**
   * */
  struct timeval tv;

  /**
   * */
  int retval, maxfd;

  /**
   * 新建一个 Socket 文件描述符
   * */
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  /**
   * 新建 address
   * */
  struct sockaddr_in server_address;
  // 初始化
  bzero(&server_address, sizeof(server_address));
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);
  server_address.sin_family = AF_INET;
  /**
   * bind Socket
   * */
  int b = bind(fd, (struct sockaddr *)&server_address, sizeof(server_address));
  if (b < 0) {
    std::cout << "绑定失败" << std::endl;
    perror("bind");
    return 0;
  }

  /**
   * 监听Socket ，并设置阻塞队列
   * */
  int l = listen(fd, QUEUE);
  if (l < 0) {
    std::cout << "监听失败" << std::endl;
    perror("listen");
    return 0;
  }

  /**
   * 给客户端分配 socket
   * */
  struct sockaddr_in client_socker;
  socklen_t client_socket_length = sizeof(client_socker);
  /**
   * accept 等待连接
   * */
  int a = accept(fd, (struct sockaddr *)&client_socker, &client_socket_length);
  if (a < 0) {
    std::cout << "accept 失败" << std::endl;
    perror("accept");
    return 0;
  }
  /**
   * 等待连接进来
   * */
  while (1) {
    // 清空可读文件
    FD_ZERO(&rdfs);
    // 标准输入文件描述符，添加到文件集合
    FD_SET(0, &rdfs);
    maxfd = 0;
    // 添加当前连接添加到集合
    FD_SET(a, &rdfs);
    if (maxfd < a) {
      maxfd = a;
    }

   // 设置超时时间
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    // 异步监听进来的连接
    retval = select(maxfd + 1, &rdfs, NULL, NULL, &tv);
    // 连接返回信息
    if (retval < 0) {
      std::cout << "select Fail" << std::endl;
      break;
    } else if (retval == 0) {
      std::cout << "read message is empty" << std::endl;
    } else {
      std::cout << "have message go up up " << std::endl;
      // 判断是否是客户发来的信息
      if (FD_ISSET(a, &rdfs)) {
        char buffer[1024];
        // 清空缓存
        bzero(&buffer, sizeof(buffer));
        int count = recv(a, buffer, sizeof(buffer), 0);
        if (count < 0) {
          std::cout << "接收信息失败" << std::endl;
        }
        std::cout << "recv message:" << buffer << std::endl;
        
        if (strcmp(buffer,"you zenme bufa message") == 0) {
          char buff[0];
          ssize_t s = send(a, buff, sizeof(buff), 0);
          if (s < 0) {
            std::cout << "send message  Fila" << std::endl;
          }
        } else {
          // 用户发送过来信息，回复信息
          if (FD_ISSET(a, &rdfs)) {
            char buff[1024] = "dsafasfasfsaf";
            ssize_t s = send(a, buff, sizeof(buff), 0);
            if (s < 0) {
              std::cout << "send message  Fila" << std::endl;
            }
          }
        }

      } else {
        std::cout << "is Not Client Send Message" << std::endl;
      }
    }
  }

  /**
   * 关闭连接
   * */
  close(a);
  /**
   * 关闭文件描述符
   * */
  close(fd);

  return 0;
}


int client(){


    int sock_cli;
    fd_set rdfs;
    struct timeval tv;

    int retval,maxfd;

    // 新建 socket 文件
    sock_cli = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(MYPORT);
    server_addr.sin_family = AF_INET;

    int connection = connect(sock_cli,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if (connection < 0 ){
        std::cout << "connection 失败" <<std::endl;
        perror("connection");
        return 0;
    }

    while (1){
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rdfs);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rdfs);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(sock_cli, &rdfs);
        /*找出文件描述符集合中最大的文件描述符*/
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(maxfd+1,&rdfs,NULL,NULL,&tv);
        if (retval == -1){
            std::cout << "select Fail" << std::endl;
            perror("select");
            continue;
        } else if(retval == 0){
            std::cout << "接收到空的信息" << std::endl;
            char buff[1024] = "you zenme bufa message";
            send(sock_cli,buff,sizeof(buff),0);
        } else{
            if (FD_ISSET(sock_cli,&rdfs)){
                char buff[1024];
                int recv_ = recv(sock_cli,buff,sizeof(buff),0);
                if (recv_ <= 0){
                    std::cout << "读取到的数据失败 " << std::endl;
                    break;
                }
                std::cout << "接收到的信息：" << buff << "长度：" << strlen(buff) << "recv_:" << recv_ << std::endl;
                memset(buff, 0, sizeof(buff));
            }
            if(FD_ISSET(0, &rdfs))
            {
                char sendbuf[BUFFER_SIZE];
                std::cout << "请输入数据" << std::endl;
                fgets(sendbuf, sizeof(sendbuf), stdin);
                send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                memset(sendbuf, 0, sizeof(sendbuf));
            }
        }

    }

    return 0;
}