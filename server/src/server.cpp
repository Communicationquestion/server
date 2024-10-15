
#include <thpool/thpool.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <server/server.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <thpool/task.h>

int Server::initserver(int _port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("socket() failed.\n");
		return -1;
	}
	int opt = 1;
	unsigned int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(_port);
	if(bind(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		printf("bind() failed.\n");
		close(sock);
		return -1;
	}
	if(listen(sock, 5) != 0) {
		printf("listen() failed.\n");
		close(sock);
		return -1;
	}
	return sock;
}
void Server::server_accept(int curfd) {
	int cfd = accept(curfd, NULL, NULL);
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flag);
	struct epoll_event ev;
	ev.data.fd = cfd;
	ev.events = EPOLLIN | EPOLLET;

	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
	if(ret == -1) {
		perror("epoll_ctl_accept");
		exit(0);
	}

}
int Server::server_run(int lfd) {
	epfd = epoll_create(100);
	if(epfd == -1) {
		perror("epoll_create");
		exit(0);
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	if(ret == -1) {
		perror("epoll_ctl");
		exit(0);
	}
	int size = sizeof(evs) / sizeof(struct epoll_event);
	while(1) {
		int num = epoll_wait(epfd, evs, size, -1);
		printf("num:%d\n", num);

		for(int i = 0; i < num; ++i) {
			int curfd = evs[i].data.fd;
			if(curfd == lfd) {
				server_accept(curfd);
			} else {
				auto _task = std::make_shared<task>(curfd, epfd);
				thpool.push_rewu_tasks(_task.get());
			}
		}
	}
}

int main() {
	Server server{};
	int serverfd = server.initserver(80);
	server.server_run(serverfd);
	return 0;
}