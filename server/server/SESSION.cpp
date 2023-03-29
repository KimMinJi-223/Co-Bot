#include "SESSION.h"

#include <iostream>

SESSION::SESSION()
	: state(state::free)
	, sock(0)
	, id(-1)
	, location(-1.f, -1.f, -1.f)
	, yaw(0.f)
	, tm_id(-1)
	, tm_location(-1.f, -1.f, -1.f)
	, tm_yaw(0.f)
	, prev_remain(0)

{
	//ZeroMemory(&recv_over.over, sizeof(recv_over.over));
	//recv_over.wsabuf.buf = recv_over.buffer;
	//recv_over.wsabuf.len = BUF_SIZE;
	//recv_over.mode = IO_RECV;
}

SESSION::~SESSION()
{
}

void SESSION::recv_packet()
{
	DWORD recv_flag = 0;
	ZeroMemory(&recv_over.over, sizeof(recv_over.over));
	recv_over.wsabuf.buf = recv_over.buffer + ring_buff.diff();
	recv_over.wsabuf.len = BUF_SIZE - ring_buff.diff();
	recv_over.mode = IO_RECV;
	WSARecv(sock, &recv_over.wsabuf, 1, 0, &recv_flag, &recv_over.over, 0);
}

void SESSION::send_packet(char* packet)
{
	OVER_EX* over = new OVER_EX;
	over->wsabuf.buf = reinterpret_cast<char*>(packet);
	over->wsabuf.len = packet[0];
	ZeroMemory(&over->over, sizeof(over->over));
	over->mode = IO_SEND;
	memcpy(over->buffer, packet, packet[0]);
	int ret = WSASend(sock, &over->wsabuf, 1, 0, 0, &over->over, 0);
	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		std::cout << err << std::endl;
	}
}

void SESSION::send_enter_packet()
{
	sc_login_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::sc_login);
	pack.id = id;
	//pack.x = x;
	//pack.y = y;
	//pack.z = z;
	//pack.yaw = yaw;
	//pack.tm_x = tm_x;
	//pack.tm_y = tm_y;
	//pack.tm_z = tm_z;
	//pack.tm_yaw = tm_yaw;
	
	send_packet(reinterpret_cast<char*>(&pack));

	printf("%d에게 enter packet을 보냈습니다.\n", id);
}

void SESSION::send_left_move_packet(int client_id)
{
	sc_move_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<int>(packet_type::sc_move);
	pack.client_id = client_id;
	pack.direction = direction::left;
	if (client_id == id) {
		pack.location = location;
		pack.current = current_left;
		pack.time = time_left;
		pack.yaw = yaw;
	} else if (client_id == tm_id) {
		pack.location = tm_location;
		pack.current = tm_current_left;
		pack.time = tm_time_left;
		pack.yaw = tm_yaw;
	}

	std::cout << client_id << "client yaw: " << pack.yaw << std::endl;

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_right_move_packet(int client_id)
{
	sc_move_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<int>(packet_type::sc_move);
	pack.client_id = client_id;
	pack.direction = direction::right;
	if (client_id == id) {
		pack.location = location;
		pack.current = current_right;
		pack.time = time_right;
		pack.yaw = yaw;
	} else if (client_id == tm_id) {
		pack.location = tm_location;
		pack.current = tm_current_right;
		pack.time = tm_time_right;
		pack.yaw = tm_yaw;
	}

	send_packet(reinterpret_cast<char*>(&pack));
}

