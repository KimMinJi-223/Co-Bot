#pragma once
#include "OBJECT.h"
#include "pch.h"

#include <mutex>

#include "RingBuffer.h"

enum class STATE {INGAME};

class SESSION : public OBJECT
{
public:
	STATE		state;
	SOCKET		sock;
	int			id;				// ∞Ì¿Ø id
	char		name[BUF_SIZE];
	RingBuffer	ring_buff;

	OVER_EX		recv_over;

public:
	void recv_packet();
	void send_packet(char* packet);

public:
	SESSION();
	~SESSION();
};

