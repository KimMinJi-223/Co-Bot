#include "ServerMain.h"

#include <MSWSock.h> // AcceptEx()
#include <vector>
#include <thread>
#include <iostream>
#include <array>
#include <mutex>
#include <memory>


#pragma comment(lib, "mswsock.lib")

#include "RingBuffer.h"
#include "SESSION.h"


std::array<SESSION, MAX_USER> clients;

ServerMain::ServerMain()
	: iocp_handle(nullptr)
	, server_sock(0)
	, client_sock(0)
{
}

ServerMain::~ServerMain()
{
	closesocket(server_sock);
	WSACleanup();
}

bool ServerMain::init()
{
	setlocale(LC_ALL, "KOREAN");

	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		GetLastError();
		return false;
	}

	iocp_handle = CreateIOCPHandle();
	if (nullptr == iocp_handle) {
		GetLastError();
		return false;
	}

	server_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	sockaddr_in server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT_NUM);

	if (SOCKET_ERROR == bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr))) {
		GetLastError();
		return false;
	}

	if (SOCKET_ERROR == listen(server_sock, SOMAXCONN)) {
		GetLastError();
		return false;
	}

	if (!AssociateSocketWithIOCP(server_sock, 7777)) {
		GetLastError();
		return false;
	}

	std::cout << "서버가 활성화 되었습니다. 접속을 기다립니다..." << std::endl;

	return true;
}

void ServerMain::server_main() // 본격적인 서버 루프 들어가는 곳
{
	client_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	overlapped.wsabuf.len = BUF_SIZE;
	overlapped.mode = IO_ACCEPT;

	AcceptEx(server_sock, client_sock, overlapped.buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, 0, &overlapped.over);

	std::vector<std::thread> worker_threads;
	int thread_num = std::thread::hardware_concurrency();
	for (int i{}; i < thread_num; ++i)
		worker_threads.emplace_back(std::thread(&ServerMain::worker_thread, this));

	for (auto& th : worker_threads)
		th.join();
}

HANDLE ServerMain::CreateIOCPHandle()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}

bool ServerMain::AssociateSocketWithIOCP(SOCKET sock, ULONG_PTR key)
{
	HANDLE h = CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock), iocp_handle, key, 0);
	return (h == iocp_handle);
}

void ServerMain::worker_thread()
{
	DWORD num_bytes;
	ULONG_PTR key;
	WSAOVERLAPPED* over = nullptr;

	while (true)
	{
		GetQueuedCompletionStatus(iocp_handle, &num_bytes, &key, &over, INFINITE);

		OVER_EX* over_ex = reinterpret_cast<OVER_EX*>(over);
		//OVER_EX* over1 = reinterpret_cast<OVER_EX*>(over);
		//std::shared_ptr<OVER_EX> over_ex = std::make_shared<OVER_EX>(over1);

		if ((0 == num_bytes) && ((IO_RECV == over_ex->mode) || (IO_SEND == over_ex->mode))) {
			/*
				연결 끊기 코드 추가해야 함
			*/

			std::cout << "num bytes: " << num_bytes << "over_ex mode: IO_RECV" << "over_ex mode: IO_SEND" << std::endl;

			if (IO_SEND == over_ex->mode)
				//delete over_ex;

				continue;
		}

		switch (over_ex->mode)
		{
		case IO_ACCEPT:
		{
			int client_id = get_client_id();

			std::cout << client_id << "번째 ID를 가진 클라가 입장하였습니다." << std::endl;

			if (client_id != -1) {
				{
					std::lock_guard<std::mutex> lock(clients[client_id].state_lock);
					clients[client_id].state = state::alloc;
				}
				/*
					유저 정보 초기화
					SESSION에 정보 추가할 때마다 여기서 초기값 정해주도록 하자
				*/
				clients[client_id].id = client_id;
				clients[client_id].sock = client_sock;
				clients[client_id].tm_id = -1;

				// 접속을 다시 받기 위해서
				if (!AssociateSocketWithIOCP(client_sock, client_id)) {
					GetLastError();
					continue;
				}

				clients[client_id].recv_packet();

				client_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
			}
			else {
				std::cout << "Max user!\n";
			}

			overlapped.wsabuf.len = BUF_SIZE;
			overlapped.mode = IO_ACCEPT;

			AcceptEx(server_sock, client_sock, overlapped.buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, 0, &overlapped.over);
		} break;
		case IO_RECV:
		{
			int remain_data = num_bytes + clients[key].prev_remain;
			char* p = over_ex->buffer;

			while (remain_data > 0)
			{
				int packet_size = p[0];
				if (packet_size <= remain_data) {
					process_packet(p, static_cast<int>(key));
					p = p + packet_size;
					remain_data = remain_data - packet_size;
				}
				else break;
			}

			clients[key].prev_remain = remain_data;

			if (remain_data > 0)
				memcpy(over_ex->buffer, p, remain_data);

			clients[key].recv_packet();

			// ring buffer
   //         RingBuffer* ring_buff = &clients[key].ring_buff;
   //         char* p = over_ex->buffer;

   //         ring_buff->lock.lock();
   //         ring_buff->enqueue(p, num_bytes);
   //         ring_buff->lock.unlock();

			//while (p[0] <= ring_buff->diff() && !ring_buff->empty())
			//{
   //             char ring_pack[BUF_SIZE];

   //             ring_buff->lock.lock();
   //             ring_buff->dequeue(reinterpret_cast<char*>(&ring_pack), p[0]);
   //             ring_buff->lock.unlock();

			//	process_packet(ring_pack, static_cast<int>(key));
			//	ring_buff->move_read_pos(p[0]);
			//	p += p[0];
			//} 

   //         if (0 < ring_buff->diff())
   //         {
   //             memcpy(over_ex->buffer, p, num_bytes);
   //         }

   //         clients[key].recv_packet();
		} break;
		case IO_SEND:
		{
			//delete over_ex;
		} break;
		}
	}
}

int ServerMain::get_client_id() // 고유한 client id 제공
{
	for (int i{}; i < MAX_USER; ++i)
	{
		std::lock_guard<std::mutex> lock{ clients[i].state_lock };
		if (clients[i].state == state::free)
			return i;
	}

	return -1;
}

void ServerMain::process_packet(char* packet, int client_id)
{
	switch (packet[1])
	{
	case static_cast<int>(packet_type::cs_login):
	{
		cs_login_packet* pack = reinterpret_cast<cs_login_packet*>(packet);
		printf("%d client로부터 login packet을 받았습니다.\n", client_id);

		wcscpy_s(clients[client_id].name, MAX_LOGIN_LEN, pack->id);
		wprintf(L"%d client의 name: %s,\n", client_id, clients[client_id].name);

		{
			std::lock_guard<std::mutex> lock{ clients[client_id].state_lock };
			clients[client_id].state = state::ingame;
		}

		bool is_matching = matching(client_id);
		if (!is_matching)
			std::cout << "이미 다른 팀원이 있습니다." << std::endl;

		set_team_position(client_id);

		clients[client_id].send_enter_packet();
	} break;
	case static_cast<int>(packet_type::cs_enter):
	{
		cs_enter_packet* pack = reinterpret_cast<cs_enter_packet*>(packet);
		printf("%d client로부터 enter packet을 받았습니다.\n", client_id);

		set_team_position(client_id);

		clients[client_id].send_enter_packet();
	} break;
	case static_cast<int>(packet_type::cs_move):
	{
		cs_move_packet* pack = reinterpret_cast<cs_move_packet*>(packet);
		//clients[client_id].last_move_time = pack->move_time;
		printf("%d ID를 가진 클라이언트가 %f, %f, %f 로 이동하였습니다.\n", client_id, pack->location.x, pack->location.y, pack->location.z);
		clients[client_id].location = pack->location;
		clients[clients[client_id].tm_id].tm_location = pack->location;

		clients[client_id].yaw = pack->yaw;
		clients[clients[client_id].tm_id].tm_yaw = pack->yaw;

		if (direction::left == pack->direction) {
			clients[client_id].current_left = pack->current;
			clients[client_id].time_left = pack->time;

			clients[clients[client_id].tm_id].tm_current_left = pack->current;
			clients[clients[client_id].tm_id].tm_time_left = pack->time;

			clients[client_id].send_left_move_packet(client_id); // 움직인 클라한테 보내기
			clients[clients[client_id].tm_id].send_left_move_packet(client_id); // 상대 클라한테 보내기
		}
		else if (direction::right == pack->direction) {
			clients[client_id].current_right = pack->current;
			clients[client_id].time_right;

			clients[clients[client_id].tm_id].tm_current_right = pack->current;
			clients[clients[client_id].tm_id].tm_time_right = pack->time;

            clients[client_id].send_right_move_packet(client_id); // 움직인 클라한테 보내기
            clients[clients[client_id].tm_id].send_right_move_packet(client_id); // 상대 클라한테 보내기
        }
    } break;
    case static_cast<int>(packet_type::cs_logout):
    {
        {
            std::lock_guard<std::mutex> lock(clients[client_id].state_lock);
            clients[client_id].state = state::free;
        }
        {
            std::lock_guard<std::mutex> llock{ clients[client_id].match_lock };
            clients[client_id].tm_id = -1;
        }
        closesocket(clients[client_id].sock);
        std::cout << client_id << "client가 logout 하였습니다.\n";
    } break;
    case static_cast<int>(packet_type::cs_push_button_maze_forward):
    {
        std::cout << client_id << " client가 forward button을 밟았습니다." << std::endl;
        clients[client_id].send_push_maze_button_packet(direction::forward);
        clients[clients[client_id].tm_id].send_push_maze_button_packet(direction::forward);
    } break;
    case static_cast<int>(packet_type::cs_push_button_maze_back):
    {
        std::cout << client_id << " client가 back button을 밟았습니다." << std::endl;
        clients[client_id].send_push_maze_button_packet(direction::back);
        clients[clients[client_id].tm_id].send_push_maze_button_packet(direction::back);
    } break;
    case static_cast<int>(packet_type::cs_push_button_maze_left):
    {
        std::cout << client_id << " client가 left button을 밟았습니다." << std::endl;
        clients[client_id].send_push_maze_button_packet(direction::left);
        clients[clients[client_id].tm_id].send_push_maze_button_packet(direction::left);
    } break;
    case static_cast<int>(packet_type::cs_push_button_maze_right):
    {
        std::cout << client_id << " client가 right button을 밟았습니다." << std::endl;
        clients[client_id].send_push_maze_button_packet(direction::right);
        clients[clients[client_id].tm_id].send_push_maze_button_packet(direction::right);
    } break;
    case static_cast<int>(packet_type::cs_push_button_gear_forward):
    {
        std::cout << "gear-------------------------------------\n";
        clients[client_id].send_push_gear_button_packet(direction::forward);
        clients[clients[client_id].tm_id].send_push_gear_button_packet(direction::forward);
    } break;
    case static_cast<int>(packet_type::cs_push_button_gear_back):
    {
        std::cout << "gear-------------------------------------\n";
        clients[client_id].send_push_gear_button_packet(direction::back);
        clients[clients[client_id].tm_id].send_push_gear_button_packet(direction::back);
    } break;
    case static_cast<int>(packet_type::cs_push_button_gear_left):
    {
        std::cout << "gear-------------------------------------\n";
        clients[client_id].send_push_gear_button_packet(direction::left);
        clients[clients[client_id].tm_id].send_push_gear_button_packet(direction::left);
    } break;
    case static_cast<int>(packet_type::cs_push_button_gear_right):
    {
        std::cout << "gear-------------------------------------\n";
        clients[client_id].send_push_gear_button_packet(direction::right);
        clients[clients[client_id].tm_id].send_push_gear_button_packet(direction::right);
    } break;
	case static_cast<int>(packet_type::cs_elevator):
	{
		cs_elevator_packet* pack = reinterpret_cast<cs_elevator_packet*>(packet);
		std::cout << "elevator: " << pack->elevator_number << std::endl;
		if (2 == pack->elevator_number) {
			clients[client_id].send_elevator_ok_packet();
			clients[clients[client_id].tm_id].send_elevator_ok_packet();
		}
	} break;
	case static_cast<int>(packet_type::cs_push_button_Forklift_red):
	{
		clients[client_id].send_forklift_button(packet_type::sc_push_button_Forklift_red);
		clients[clients[client_id].tm_id].send_forklift_button(packet_type::sc_push_button_Forklift_red);
	} break;
	case static_cast<int>(packet_type::cs_push_button_Forklift_green):
	{
		clients[client_id].send_forklift_button(packet_type::sc_push_button_Forklift_green);
		clients[clients[client_id].tm_id].send_forklift_button(packet_type::sc_push_button_Forklift_green);
	} break;
	case static_cast<int>(packet_type::cs_push_button_Forklift_blue):
	{
		clients[client_id].send_forklift_button(packet_type::sc_push_button_Forklift_blue);
		clients[clients[client_id].tm_id].send_forklift_button(packet_type::sc_push_button_Forklift_blue);
	} break;
	case static_cast<int>(packet_type::cs_push_button_Forklift_black):
	{
		clients[client_id].send_forklift_button(packet_type::sc_push_button_Forklift_black);
		clients[clients[client_id].tm_id].send_forklift_button(packet_type::sc_push_button_Forklift_black);
	} break;
	case static_cast<int>(packet_type::cs_push_button_cobot_red):
	{
		clients[client_id].send_cobot_button(packet_type::sc_push_button_cobot_red);
		clients[clients[client_id].tm_id].send_cobot_button(packet_type::sc_push_button_cobot_red);
	} break;
	case static_cast<int>(packet_type::cs_push_button_cobot_green):
	{
		clients[client_id].send_cobot_button(packet_type::sc_push_button_cobot_green);
		clients[clients[client_id].tm_id].send_cobot_button(packet_type::sc_push_button_cobot_green);
	} break;
	case static_cast<int>(packet_type::cs_push_button_cobot_blue):
	{
		clients[client_id].send_cobot_button(packet_type::sc_push_button_cobot_blue);
		clients[clients[client_id].tm_id].send_cobot_button(packet_type::sc_push_button_cobot_blue);
	} break;
	case static_cast<int>(packet_type::cs_push_button_cobot_black):
	{
		clients[client_id].send_cobot_button(packet_type::sc_push_button_cobot_black);
		clients[clients[client_id].tm_id].send_cobot_button(packet_type::sc_push_button_cobot_black);
	} break;
	case static_cast<int>(packet_type::cs_start_time_color):
	{
		// timer thread로 바꾸자 다음에
		std::cout << "recv cs_start_time_color" << std::endl;
		//static int count = 0;
		//static std::mutex m;
		//m.lock();
		//++count;
		////m.unlock();
		//if (4 == count) {
		int color = rand() % 8;
		clients[client_id].send_board_color(color);
		clients[clients[client_id].tm_id].send_board_color(color);
		//	m.lock();
		//	count = 0;
		//	m.unlock();
		//} else if (count > 4) {
		//	std::cout << "count: " << count << std::endl;
		//}
	} break;
	}
}

bool ServerMain::matching(int client_id)
{
	for (int i{}; ; ++i)
	{
		if (MAX_USER == i) i = 0;
		if (state::ingame != clients[i].state || clients[i].id == client_id) continue;

		clients[client_id].match_lock.lock();
		if (-1 == clients[client_id].tm_id) {
			clients[i].match_lock.lock();
			if (-1 == clients[i].tm_id) {
				clients[client_id].tm_id = clients[i].id;
				clients[i].tm_id = client_id;
				clients[i].match_lock.unlock();
				clients[client_id].match_lock.unlock();

				std::cout << i << ", " << client_id << "matching!" << std::endl;

				return true;
			}
			else {
				clients[i].match_lock.unlock();
			}
		}
		else { // 이미 팀원이 있다는 소리
			clients[client_id].match_lock.unlock();

			return false;
		}
	}
}

void ServerMain::set_team_position(int client_id)
{
	clients[client_id].tm_location = clients[clients[client_id].tm_id].location;
}