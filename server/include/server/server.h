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
#include <thpool/task.h>
#include <queue>

class Server {
public:
	Server(int c);
	int initserver(int _port);
	int server_run(int lfd);
	void server_accept(int curfd);
	void read_ev(int curfd,int epfd,std::string res);
private:
	struct sockaddr_in servaddr;
	int epfd;
	//MyTask task;
	std::queue<MyTask> tasks;
	struct epoll_event evs[1024];
	std::unique_ptr<ThreadPool> threadpool_;


};