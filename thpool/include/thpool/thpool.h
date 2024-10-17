#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <string>
#include <atomic>
#include <unistd.h>
#include <thpool/task.h>

class threadpool {
public:
	threadpool(int count) : tpfg(true) {
		for(int i = 0; i < count; i++) {
			threads.emplace_back(&threadpool::work, this);
		}
	}
	~threadpool() {
		threadstop();
	}
	void threadstart() {
		tpfg = true;
	}
	void threadstop() {
		tpfg = false;
		cv.notify_all();
		for(auto& th : threads) {
			if(th.joinable()) {
				th.join();
			}
		}
	}

	void  push_rewu_tasks(task taskone) {
		std::unique_lock<std::mutex> ulock(this->mtx);
		this->tasks.push(taskone);
		ulock.unlock();
		this->cv.notify_one();
	}
	void work() {
		while(true) {
			if(!tpfg) {
				return;
			}
			task* taskone = nullptr;
			{
				std::unique_lock<std::mutex> lock(this->mtx);
				while(tasks.empty()) {
					cv.wait(lock, [&] { return !tasks.empty() || !tpfg; });
				}
				if(tpfg) {
					taskone = &this->tasks.front();
					this->tasks.pop();

				}
				if(taskone != nullptr) {

					taskone->run();

				}

			}

		}
	}

private:
	std::vector<std::thread> threads;
	std::queue<task> tasks;
	std::atomic_bool tpfg;
	std::condition_variable cv;
	std::mutex mtx;
};
