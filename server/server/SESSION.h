#pragma once
#include "OBJECT.h"
#include "pch.h"

#include <mutex>

#include "RingBuffer.h"

enum class state { free, alloc, ingame };

class SESSION : public OBJECT
{
public:
	state		state;
	SOCKET		sock;
	int			id;				// °íÀ¯ id
	char		name[BUF_SIZE];
	double		x, y, z;
	double		yaw;
	std::mutex	lock;
	RingBuffer	ring_buff;

	OVER_EX		recv_over;

	int			tm_id;	// ÆÀ¿ø id
	double		tm_x, tm_y, tm_z;
	double		tm_yaw;

public:
	void recv_packet();
	void send_packet(char* packet);
	void send_login_packet();
	void send_move_packet(int client_id);
	void send_rotate_packet(int client_id);

public:
	SESSION();
	~SESSION();
};

