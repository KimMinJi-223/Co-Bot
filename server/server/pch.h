#pragma once

#include <WinSock2.h>
#include <memory>
#include <iostream>
#include <chrono>
#include "protocol.h"

#pragma comment(lib, "ws2_32.lib")

enum COMP_MODE { IO_RECV, IO_SEND, IO_ACCEPT, MOVE_CAR };
enum event_type { move_car };

struct OVER_EX {
	WSAOVERLAPPED   over;
	WSABUF          wsabuf;
	char            buffer[BUF_SIZE];
	COMP_MODE       mode;
	int				object_id;

public:
	OVER_EX()
	{
		ZeroMemory(&over, sizeof(over));
		wsabuf.buf = buffer;
		object_id = -1;
	}

	OVER_EX(OVER_EX& over_ex)
	{
		ZeroMemory(&over, sizeof(over));
		over_ex.wsabuf.buf = buffer;
		object_id = -1;
	}
};

struct TIMER_EVENT {
	int object_id;
	std::chrono::system_clock::time_point execute_time;
	event_type event_type;

	constexpr bool operator < (const TIMER_EVENT& L) const
	{
		return (execute_time > L.execute_time);
	}
};