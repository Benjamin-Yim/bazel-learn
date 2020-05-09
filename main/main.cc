
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <list>
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

int fd;
struct sockaddr_in server_addr;
socklen_t socket_length;
std::list<int> conns;

void accept_fun() {
  while (1) {
    int acc = accept(fd, (struct sockaddr *)&server_addr, &socket_length);
    conns.push_back(acc);
    std::cout << "accept value:" << acc << std::endl;
  }
}

void select_data() {
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  while (1) {

    std::list<int>::iterator item_iterator;
    for (item_iterator = conns.begin(); item_iterator != conns.end();
         ++item_iterator) {
      fd_set rfds;
      FD_SET(0, &rfds);
      FD_SET(*item_iterator, &rfds);

      int recval = select(*item_iterator+1, &rfds, NULL, NULL, &tv);
      if (recval < 0) {
        std::cout << "select fail:" << *item_iterator << std::endl;
        break;
      } else if (recval == 0) {
        std::cout << "recv message is EMPTY" << std::endl;
        continue;
      } else {
        // if (FD_ISSET(*item_iterator, &rfds)) {
          
        // }
        char buffer[1024];
          int content = recv(*item_iterator, &buffer, sizeof(buffer), 0);
          if (content <= 0) {
            std::cout << "client close socket:" << *item_iterator << std::endl;
            conns.remove(*item_iterator);
            continue;
          }

          std::cout << "recv message :" << buffer << std::endl;
      }
    }
  }
}

void send_message() {
  while (1) {
    char buffer[1024] = "服务方来信";
    std::list<int>::iterator item;
    for (item = conns.begin(); item != conns.end(); ++item) {
      send(*item, &buffer, sizeof(buffer), 0);
    }
    sleep(3);
  }
}

int main(int argc, char **argv) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_family = AF_INET;

  int b = bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (b < 0) {
    std::cout << "绑定失败" << std::endl;
    return 0;
  }
  int l = listen(fd, QUEUE);
  if (l < 0) {
    std::cout << "监听失败" << std::endl;
    return 0;
  }
  std::thread t(accept_fun);
  t.detach();
  std::thread t1(select_data);
  t1.detach();
  std::thread t2(send_message);
  t2.detach();
  while (1) {
    /* code */
  }

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
                printf("%s\n", recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));
                char sendbuf[BUFFER_SIZE] = "message 2222222222222222";
                send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                memset(sendbuf, 0, sizeof(sendbuf));
                sleep(1);
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