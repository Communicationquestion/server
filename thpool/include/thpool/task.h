#pragma once
#include<http/http.h>
class task
{
public:
    task(int curfd, int epfd,std::string &str) :cfd(curfd), _epfd(epfd),req_str(str) {};
    ~task();
     int test();

	 void run();

private:

    int cfd;
    int _epfd;
    std::string req_str;
   
};
