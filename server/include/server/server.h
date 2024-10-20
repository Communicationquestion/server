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
	Server() {


	}
	int initserver(int _port);
	int server_run(int lfd);
	void server_accept(int curfd);

private:
	struct sockaddr_in servaddr;
	int epfd;

	std::queue<MyTask> tasks;
	struct epoll_event evs[1024];
	ThreadPool tp;


};