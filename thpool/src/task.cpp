#include <thpool/task.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<future>
#include <functional>
std::mutex qmutex;
MyTask::~MyTask() {

	try {
		if(epoll_ctl(_epfd, EPOLL_CTL_DEL, cfd, nullptr) == -1) {
			perror("epoll_ctl: EPOLL_CTL_DEL");
		}
	} catch(const std::exception& e) {
		e.what();
	}

	close(cfd);
}

int MyTask::test() {

	return 0;
}

void MyTask::run()  {


	std::cout << "cfd: " << cfd << "\n" << "_epfd: " << _epfd << std::endl;

	std::cout << "task run \n" << req_str << std::endl;


	
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//auto future = std::async(std::launch::async, [=]() {
	

	
		//std::lock_guard<std::mutex> lock(qmutex);

		Http http(cfd, req_str);
		http.accept_request();
		

	
		
	
	//});
	//future.share();



	return ;


}
