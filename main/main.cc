
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

#define PORT 7000
#define QUEUE 20

int ss;
struct sockaddr_in client_addr;
socklen_t length = sizeof(client_addr);
int conns[2] =
    {}; //定义了一个容量为2的数组来存放套接字，所以server最多只能跟2个client通信
int z = 0;

void thread_fn() {
  //成功返回非负描述字，出错返回-1
  int conn = accept(ss, (struct sockaddr *)&client_addr, &length);
  if (conn < 0) {
    perror("connect");
    exit(1);
  }

  //把连接保存到临时数组中;
  conns[z] = conn;
  z++;

  fd_set rfds;
  struct timeval
      tv; // linux编程中，如果用到计时，可以用struct timeval获取系统时间
  int retval, maxfd;
  while (1) {
    /*把可读文件描述符的集合清空*/
    FD_ZERO(&rfds);
    /*把标准输入的文件描述符加入到集合中*/
    FD_SET(0, &rfds);
    maxfd = 0;
    /*把当前连接的文件描述符加入到集合中*/
    FD_SET(conn, &rfds);
    /*找出文件描述符集合中最大的文件描述符*/
    if (maxfd < conn) {
      maxfd = conn;
    }
    /*设置超时时间*/
    tv.tv_sec = 5; // 5秒
    tv.tv_usec = 0;
    /*等待聊天*/
    retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("select fail\n");
      break;
    } else if (retval == 0) {
      printf("message is empty，waiting...\n");
      // 加上这个无法接收到数据  需要查明原因
      // for (int i = 0; i < z; i++) {
      //   char buff[1024];
      //   sprintf(buff, "%s%d", "你怎么不发信息啊", conns[i]);
      //   send(conns[i], buff, sizeof(buff), 0);
      // }
      continue;
    } else {
      /*客户端发来了消息*/
      if (FD_ISSET(conn, &rfds)) //判断conn是否在rfds中如果在返回非零，不再返回0
      {
        char buffer[1024];
        memset(buffer, 0,
               sizeof(buffer)); //把buffer中的所有值赋值为0，即清空buffer
        int len = recv(conn, buffer, sizeof(buffer),
                       0); //把接收到的数据存放于buffer中
        if (len <= 0) {
          std::cout << "recv message fail" << std::endl;
          break;
        }
        std::cout << "accept:" << conn << ",recv message :" << buffer
                  << std::endl;
      }
      /*用户输入信息了,开始处理信息并发送*/
      if (FD_ISSET(0, &rfds)) {
        char buf[1024] = "该你发信息了";
        for (int i = 0; i < z; i++) {
          send(conns[i], buf, sizeof(buf), 0);
        }
      }
    }
  }
  close(conn);
}

int main(int argc, char **argv) {
  ss = socket(AF_INET, SOCK_STREAM,
              0); // SOCK_STREAM即tcp协议，AF_INET是IPv4套接字
  struct sockaddr_in server_sockaddr;
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = htons(PORT);
  server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(ss, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) ==
      -1) {
    perror("bind");
    exit(1);
  }
  if (listen(ss, QUEUE) == -1) {
    perror("listen");
    exit(1);
  }
  std::thread t(thread_fn); //因为创建了两个线程所以只能连接两个client
  std::thread t1(
      thread_fn); //这里把收发数据都存放在thread_fn中，所以创建一个这样的线程就能使得server能多连接一个server
  t.join();
  t1.join();
  close(ss);
  return 0;
}



#define MYPORT  7000
#define BUFFER_SIZE 1024
int client()
{
    int sock_cli;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    //连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    while(1){
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rfds);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(sock_cli, &rfds);
        /*找出文件描述符集合中最大的文件描述符*/
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        /*设置超时时间*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*等待聊天*/
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            printf("select出错，客户端程序退出\n");
            break;
        }else if(retval == 0)
        {
            printf("客户端没有任何输入信息，并且服务器也没有信息到来，waiting...\n");
            char sendbuf[BUFFER_SIZE] = "message 2222222222222222";
            send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
            memset(sendbuf, 0, sizeof(sendbuf));
            sleep(1);
            continue;
        }
        else
        {
            /*服务器发来了消息*/
            if(FD_ISSET(sock_cli,&rfds))
            {
                char recvbuf[BUFFER_SIZE];
                int len;
                len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                printf("%s", recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));
            }
            /*用户输入信息了,开始处理信息并发送*/
            if(FD_ISSET(0, &rfds))
            {
                char sendbuf[BUFFER_SIZE] = "message 2222222222222222";
                send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                memset(sendbuf, 0, sizeof(sendbuf));
                sleep(1);
            }
        }
    }

    close(sock_cli);
    return 0;
}