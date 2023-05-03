#include "SESSION.h"

#include <iostream>
#include <memory>

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
	// std::atomic<std::shared_ptr<OVER_EX>> over;
	// std::shared_ptr<OVER_EX> over(new OVER_EX());
	// OVER_EX* over = new OVER_EX;
	// over->wsabuf.buf = reinterpret_cast<char*>(packet);

	over->wsabuf.buf = over->buffer;
	over->wsabuf.len = packet[0];
	over->mode = IO_SEND;
	ZeroMemory(&over->over, sizeof(over->over));
	memcpy(over->buffer, packet, packet[0]);
	int ret = WSASend(sock, &over->wsabuf, 1, 0, 0, &over->over, 0);
	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		std::cout << "err: " << err << std::endl;
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

void SESSION::send_push_maze_button_packet(direction direction)
{
	sc_button_packet pack;
	pack.size = sizeof(pack);
	if (direction::forward == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_maze_forward);
	} else if (direction::back == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_maze_back);
	} else if (direction::left == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_maze_left);
	} else if (direction::right == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_maze_right);
	}
	
	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_push_gear_button_packet(direction direction)
{
	sc_button_packet pack;
	pack.size = sizeof(pack);
	if (direction::forward == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_gear_forward);
	} else if (direction::back == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_gear_back);
	} else if (direction::left == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_gear_left);
	} else if (direction::right == direction) {
		pack.type = static_cast<char>(packet_type::sc_push_button_gear_right);
	}

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_elevator_ok_packet()
{
	sc_elevator_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::sc_elevator);

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_forklift_button(packet_type type)
{
	sc_button_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(type);

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_cobot_button(packet_type type)
{
	sc_button_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(type);

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_board_color(int color)
{
	sc_board_color_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::sc_board_color);
	pack.color = color;

	send_packet(reinterpret_cast<char*>(&pack));
}

void SESSION::send_logout_packet()
{
	sc_logout_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::sc_logout);

	send_packet(reinterpret_cast<char*>(&pack));
}

// 23.4.14
// 1. OVER_EX 동적 할당해주는 부분에서 메모리 릭이 있는 듯하다.
// 2. std::atomic<std::shared_ptr>에 대해서 더 조사해보기
//		- C++20에서 나오기로 했던 atomic_shared_ptr인지 확인해보기
//		- 다르면 어떤 부분이 다른지 알아보기(성능, 비용, 안정성, ...)
//		- 안전하다고 확인되면 적용시키기(이것또한 폭풍 조사,,,)
// 3. 매칭관련 부분도 다시 살펴봐야 한다.
//		- lock을 사용하고 있는데 더 좋은 방법이 없나 찾아보기
//		- 알고리즘에도 이상이 없는지 확인해보기
// 4. 링버퍼 사용한 패킷 재조립 구현하기
//		- 문제가 있는듯해서 사용하지 않았음
//		- 정확한 문제를 파악하고 고치기                                    