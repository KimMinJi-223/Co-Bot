#pragma once

#include <WinSock2.h>
#include <memory>
#include "protocol.h"

#pragma comment(lib, "ws2_32.lib")

enum COMP_MODE { IO_RECV, IO_SEND, IO_ACCEPT };

struct OVER_EX {
	WSAOVERLAPPED   over;
	WSABUF          wsabuf;
	char            buffer[BUF_SIZE];
	COMP_MODE       mode;

public:
	OVER_EX()
	{
		ZeroMemory(&over, sizeof(over));
		wsabuf.buf = buffer;
	}

	OVER_EX(OVER_EX& over_ex)
	{
		ZeroMemory(&over, sizeof(over));
		over_ex.wsabuf.buf = buffer;
	}
};