#pragma once
#include "OBJECT.h"
#include "pch.h"

#include <mutex>

#include "RingBuffer.h"

enum class STATE { NOT_INGAME, INGAME };

class SESSION : public OBJECT
{
public:
	STATE		state;
	SOCKET		sock;
	int			id;				// ∞Ì¿Ø id
	char		name[BUF_SIZE];
	double		x, y, z;
	RingBuffer	ring_buff;

	OVER_EX		recv_over;

public:
	void recv_packet();
	void send_packet(char* packet);
	void send_login_packet();
	void send_add_player(int client_id, double x, double y, double z);

public:
	SESSION();
	~SESSION();
};

