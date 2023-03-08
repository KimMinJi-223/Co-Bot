#include "ServerMain.h"

int main()
{
	ServerMain server;

	if (server.init()) // 초기화가 성공했다면 루프 돌기
	{
		server.server_main();
	}
}

//#include <iostream>
//#include "RingBuffer.h"
//#include <thread>
//#include <vector>
//#include <mutex>
//
//RingBuffer ring_buff;
//std::mutex lock;
//
//void work(int i)
//{
//	int num = i;
//	char word[9] = "working!";
//
//	while (true)
//	{
//		ring_buff.lock.lock();
//		int ret = ring_buff.enqueue(reinterpret_cast<char*>(&word), sizeof(word));
//		ring_buff.lock.unlock();
//		printf("%d\n", ret);
//
//		char dest[BUFFER_SIZE];
//		ring_buff.lock.lock();
//		ret = ring_buff.dequeue(dest, 9);
//		ring_buff.lock.unlock();
//		printf("%s\n", dest);
//	}
//}
//
//int main()
//{
//	std::vector<std::thread> workers;
//	for (int i{}; i < 5; ++i)
//	{
//		workers.push_back(std::thread(work, i));
//	}
//
//	for (int i{}; i < 5; ++i)
//	{
//		workers[i].join();
//	}
//}