#pragma once
#include<http/http.h>
#include<thpool/threadpool.h>
class MyTask 
        
{
public:
    MyTask( int curfd, int epfd,std::string &str) :cfd(curfd), _epfd(epfd),req_str(str) {};

    ~MyTask();
   
     int test();
    
	 void run() ;

private:

    int cfd;
    int _epfd;
    std::string req_str;
   
};
