#pragma once
#include<http/http.h>
class task
{
public:
    task(int curfd, int epfd) :cfd(curfd), _epfd(epfd) {};
    ~task();
     int test();
	 void run();

private:

    int cfd;
    int _epfd;
   
};
