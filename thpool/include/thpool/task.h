#pragma once
#include<http/http.h>
#include<thpool/threadpool.h>
class MyTask : public Task
        
{
public:
    MyTask( int curfd, int epfd,std::string &str) :cfd(curfd), _epfd(epfd),req_str(str) {};

    ~MyTask();
    bool operator==(const Task* task) {
		if(this == task) {
			return true;
		}
		const MyTask* p = dynamic_cast<const MyTask*>(task);
		return p != nullptr && this->cfd == p->cfd;
	};
     int test();
    
	 void run() ;

private:

    int cfd;
    int _epfd;
    std::string req_str;
   
};
