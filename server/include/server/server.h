#pragma once
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <thpool/thpool.h>
class Server {
public:
  Server(){
    
    thpool.threadstart();
  }
  int initserver(int _port);
  int server_run(int lfd);
  void server_accept(int curfd);

private:
  struct sockaddr_in servaddr;
  int epfd;
  
  struct epoll_event evs[1024];
  threadpool thpool{5};
  
};