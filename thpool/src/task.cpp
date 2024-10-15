#include <thpool/task.h>
#include<sys/epoll.h>
#include<unistd.h>
task::~task() {
	
	try {
		if(epoll_ctl(_epfd, EPOLL_CTL_DEL, cfd, nullptr) == -1) {
			perror("epoll_ctl: EPOLL_CTL_DEL");
		}
	} catch(const std::exception& e) {
		e.what();
	}

	close(cfd);
}

int task::test(){

return 0;
}

void task::run() {
	Http http(cfd);
	http.accept_request();
}
