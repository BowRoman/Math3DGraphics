#include <iostream>
#include <thread>
#include <mutex>

std::mutex lock;

const int kMax = 10;

void thread_main(int i )
{
	lock.lock();
	std::cout << "dank meme " << i << std::endl;
	lock.unlock();
}

void main()
{

	std::thread t[kMax];
	for (uint32_t i = 0; i < kMax; ++i)
	{
		t[i] = std::thread(thread_main, i);
	}
	
	lock.lock();
	std::cout << "I am the main thread" << std::endl;
	lock.unlock();


	for (uint32_t i = 0; i < kMax; ++i)
	{
		t[i].join();
	}
}