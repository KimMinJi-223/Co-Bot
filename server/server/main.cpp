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
//
//int main()
//{
//	RingBuffer ring_buffer;
//
//	char arr[] = "hello, world!";
//
//	//std::cout << sizeof(arr) << std::endl;
//
//	ring_buffer.enqueue(arr, sizeof(arr));
//
//	char dest[14];
//	ring_buffer.dequeue(dest, 14);
//
//	for (int i{}; i < 14; ++i)
//	{
//		std::cout << dest[i];
//	}
//
//	std::cout << "\n";
//
//	char arr2[] = "kimchaemin";
//
//	ring_buffer.enqueue(arr2, sizeof(arr2));
//	
//	char dest2[11];
//
//	ring_buffer.dequeue(dest2, 11);
//
//	for (int i{}; i < 11; ++i)
//	{
//		std::cout << dest2[i];
//	}
//}