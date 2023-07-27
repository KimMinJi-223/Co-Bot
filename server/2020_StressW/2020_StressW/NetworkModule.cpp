#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <winsock.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <chrono>
#include <queue>
#include <array>
#include <memory>

using namespace std;
using namespace chrono;

extern HWND		hWnd;

const static int MAX_TEST = 1000;
const static int MAX_CLIENTS = MAX_TEST * 2;					// 서버에서 접속을 거부할 수 있는데 그때 다시 접속해줘야 한다.
const static int INVALID_ID = -1;
const static int MAX_PACKET_SIZE = 255;
const static int MAX_BUFF_SIZE = 255;

#pragma comment (lib, "ws2_32.lib")

#include "C:\Users\chamcham\Desktop\plz\cobot.ver2\TCM_Cobot\server\server\protocol.h"

HANDLE g_hiocp;

enum COMP_MODE { IO_RECV, IO_SEND, IO_ACCEPT, MOVE_CAR };

high_resolution_clock::time_point last_connect_time;

//struct OverlappedEx {
//	WSAOVERLAPPED over;
//	WSABUF wsabuf;
//	unsigned char IOCP_buf[MAX_BUFF_SIZE];
//	OPTYPE event_type;
//	int event_target;
//};

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

struct CLIENT {
	int id;
	double x;
	double y;
	double z;
	atomic_bool connected; // 이 클라이언트가 지금 연결되어 있는가를 알기 위해

	SOCKET client_socket;
	OVER_EX recv_over;
	unsigned char packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;
	int curr_packet_size;
	high_resolution_clock::time_point last_move_time; // 렉 걸리는 것을 알기 위한 last_move_time.
};

array<int, MAX_CLIENTS> client_map; // 서버의 경우 인덱스의 배열이 id 이다. 더미 클라는 이 아이디와 서버에서 부여하는 아이디랑 같다는 보장이 없다. 서버가 관리하는 아이디 값이다.
array<CLIENT, MAX_CLIENTS> g_clients;
atomic_int num_connections; //
atomic_int client_to_close; // 렉이 바로 반영되지 않는다. 5초, 10초 뒤에 반영된다. 일단 다 접속시키고 끊으면서 측정한다. 그때 사용하는 것이다. // 먼저 접속한 더미 클라부터 연결을 끊는다. 접속중인데 끊길 수도 있으니까
atomic_int active_clients; // 현재 활동중인 더미 클라들

// 렉을 여기에 등록을 한다.
// 조금씩 올라가고 조금씩 내려간다. -> 평균이 반영된다.
int			global_delay;				// ms단위, 1000이 넘으면 클라이언트 증가 종료

vector <thread*> worker_threads;
thread test_thread;

float point_cloud[MAX_TEST * 2]; // 스레드 간의 커뮤니케이션하는 배열


// 나중에 NPC까지 추가 확장 용
struct ALIEN {
	int id;
	int x, y;
	int visible_count;
};

void error_display(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"에러" << lpMsgBuf << std::endl;

	MessageBox(hWnd, lpMsgBuf, L"ERROR", 0);
	LocalFree(lpMsgBuf);
	// while (true);
}

void DisconnectClient(int ci)
{
	bool status = true;
	if (true == atomic_compare_exchange_strong(&g_clients[ci].connected, &status, false)) {
		closesocket(g_clients[ci].client_socket);
		active_clients--;
	}
	// cout << "Client [" << ci << "] Disconnected!\n";
}

void SendPacket(int cl, void* packet)
{
	int psize = reinterpret_cast<unsigned char*>(packet)[0];
	int ptype = reinterpret_cast<unsigned char*>(packet)[1];
	OVER_EX* over = new OVER_EX;
	over->mode = IO_SEND;
	memcpy(over->buffer, packet, psize);
	ZeroMemory(&over->over, sizeof(over->over));
	over->wsabuf.buf = reinterpret_cast<CHAR*>(over->buffer);
	over->wsabuf.len = psize;
	int ret = WSASend(g_clients[cl].client_socket, &over->wsabuf, 1, NULL, 0,
		&over->over, NULL);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			error_display("Error in SendPacket:", err_no);
	}
	// std::cout << "Send Packet [" << ptype << "] To Client : " << cl << std::endl;
}

void ProcessPacket(int ci, unsigned char packet[])
{
	switch (packet[1]) {
		case static_cast<int>(packet_type::sc_move): {
			sc_move_packet* move_packet = reinterpret_cast<sc_move_packet*>(packet);
		if (move_packet->client_id < MAX_CLIENTS) {
			int my_id = client_map[move_packet->client_id];
			if (-1 != my_id) {
				g_clients[my_id].x = move_packet->location.x;
				g_clients[my_id].y = move_packet->location.y;
			}
			if (ci == my_id) {
				if (0 != move_packet->move_time) {
					auto d_ms = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() - move_packet->move_time;
					
					if (global_delay < d_ms) global_delay++;
					else if (global_delay > d_ms) global_delay--;
				}
			}
		}
	} break;
	//case SC_ADD_PLAYER: break;
	//case SC_REMOVE_PLAYER: break;
	case static_cast<int>(packet_type::sc_login_success):
	{
		srand((unsigned int)time(NULL));
		double rand_x = rand() % 2000;
		double rand_y = rand() % 2000;
		double rand_z = rand() % 2000;
		g_clients[ci].connected = true;
		active_clients++;
		sc_login_success_packet* login_packet = reinterpret_cast<sc_login_success_packet*>(packet);
		int my_id = ci;
		client_map[login_packet->id] = my_id;
		g_clients[my_id].id = login_packet->id;
		g_clients[my_id].x = rand_x;
		g_clients[my_id].y = rand_y;
		g_clients[my_id].z = rand_z;

		//cs_packet_teleport t_packet;
		//t_packet.size = sizeof(t_packet);
		//t_packet.type = CS_TELEPORT;
		//SendPacket(my_id, &t_packet);
	}
	break;
	default: break; // MessageBox(hWnd, L"Unknown Packet Type", L"ERROR", 0);
		while (true);
	}
}

void Worker_Thread()
{
	while (true) {
		DWORD io_size;
		unsigned long long ci;
		OVER_EX* over;
		BOOL ret = GetQueuedCompletionStatus(g_hiocp, &io_size, &ci,
			reinterpret_cast<LPWSAOVERLAPPED*>(&over), INFINITE);
		// std::cout << "GQCS :";
		int client_id = static_cast<int>(ci);
		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			if (64 == err_no) DisconnectClient(client_id);
			else {
				// error_display("GQCS : ", WSAGetLastError());
				DisconnectClient(client_id);
			}
			if (IO_SEND == over->mode) delete over;
		}
		if (0 == io_size) {
			DisconnectClient(client_id);
			continue;
		}
		if (IO_RECV == over->mode) {
			//std::cout << "RECV from Client :" << ci;
			//std::cout << "  IO_SIZE : " << io_size << std::endl;
			char* buf = g_clients[ci].recv_over.buffer;
			unsigned psize = g_clients[ci].curr_packet_size;
			unsigned pr_size = g_clients[ci].prev_packet_data;
			while (io_size > 0) {
				if (0 == psize) psize = buf[0];
				if (io_size + pr_size >= psize) {
					// 지금 패킷 완성 가능
					unsigned char packet[MAX_PACKET_SIZE];
					memcpy(packet, g_clients[ci].packet_buf, pr_size);
					memcpy(packet + pr_size, buf, psize - pr_size);
					ProcessPacket(static_cast<int>(ci), packet);
					io_size -= psize - pr_size;
					buf += psize - pr_size;
					psize = 0; pr_size = 0;
				}
				else {
					memcpy(g_clients[ci].packet_buf + pr_size, buf, io_size);
					pr_size += io_size;
					io_size = 0;
				}
			}
			g_clients[ci].curr_packet_size = psize;
			g_clients[ci].prev_packet_data = pr_size;
			DWORD recv_flag = 0;
			int ret = WSARecv(g_clients[ci].client_socket,
				&g_clients[ci].recv_over.wsabuf, 1,
				NULL, &recv_flag, &g_clients[ci].recv_over.over, NULL);
			if (SOCKET_ERROR == ret) {
				int err_no = WSAGetLastError();
				if (err_no != WSA_IO_PENDING)
				{
					//error_display("RECV ERROR", err_no);
					DisconnectClient(client_id);
				}
			}
		}
		else if (IO_SEND == over->mode) {
			if (io_size != over->wsabuf.len) {
				// std::cout << "Send Incomplete Error!\n";
				DisconnectClient(client_id);
			}
			delete over;
		}
		else if (MOVE_CAR == over->mode) {
			// Not Implemented Yet
			delete over;
		}
		else {
			std::cout << "Unknown GQCS event!\n";
			while (true);
		}
	}
}

constexpr int DELAY_LIMIT = 100;
constexpr int DELAY_LIMIT2 = 150;
constexpr int ACCEPT_DELY = 50;

void Adjust_Number_Of_Client()
{
	static int delay_multiplier = 1;
	static int max_limit = MAXINT;
	static bool increasing = true;

	if (active_clients >= MAX_TEST) return; // 지금 active 한 클라가 최대 동접보다 크거나 같다. -> return
	if (num_connections >= MAX_CLIENTS) return; // 알고즘의 한계에 부딪힌것이다. 서버가 많이 튕겨내서 클라가 2명씩 했는데도 최대에 도달한 것이다.

	auto duration = high_resolution_clock::now() - last_connect_time;
	if (ACCEPT_DELY * delay_multiplier > duration_cast<milliseconds>(duration).count()) return;

	int t_delay = global_delay; // global_delay 를 봐서 렉이 심하면 연결을 끊어야 한다.
	if (DELAY_LIMIT2 < t_delay) {
		if (true == increasing) {
			// 더 이상 늘리면 안되겠다!
			max_limit = active_clients;
			increasing = false;
		}
		if (100 > active_clients) return;
		if (ACCEPT_DELY * 10 > duration_cast<milliseconds>(duration).count()) return;
		last_connect_time = high_resolution_clock::now();
		DisconnectClient(client_to_close);
		client_to_close++;
		return;
	}
	else
		if (DELAY_LIMIT < t_delay) { // delay 를 너무 줄이면 다시 늘리는 것이다. => 줄였다 늘렸다를 반복한다.
			delay_multiplier = 10; 
			return;
		}
	if (max_limit - (max_limit / 20) < active_clients) return;

	increasing = true; // 동접이 다시 늘어난다.
	last_connect_time = high_resolution_clock::now();
	g_clients[num_connections].client_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT_NUM);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	int Result = WSAConnect(g_clients[num_connections].client_socket, (sockaddr*)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	if (0 != Result) {
		error_display("WSAConnect : ", GetLastError());
	}

	g_clients[num_connections].curr_packet_size = 0;
	g_clients[num_connections].prev_packet_data = 0;
	ZeroMemory(&g_clients[num_connections].recv_over, sizeof(g_clients[num_connections].recv_over));
	g_clients[num_connections].recv_over.mode = IO_RECV;
	g_clients[num_connections].recv_over.wsabuf.buf =
		reinterpret_cast<CHAR*>(g_clients[num_connections].recv_over.buffer);
	g_clients[num_connections].recv_over.wsabuf.len = sizeof(g_clients[num_connections].recv_over.buffer);

	DWORD recv_flag = 0;
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[num_connections].client_socket), g_hiocp, num_connections, 0);

	cs_login_packet l_packet;

	int temp = num_connections;
	sprintf_s(l_packet.id, "%d", temp);
	l_packet.size = sizeof(l_packet);
	l_packet.type = static_cast<char>(packet_type::cs_login);
	SendPacket(num_connections, &l_packet);


	int ret = WSARecv(g_clients[num_connections].client_socket, &g_clients[num_connections].recv_over.wsabuf, 1,
		NULL, &recv_flag, &g_clients[num_connections].recv_over.over, NULL);
	if (SOCKET_ERROR == ret) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			error_display("RECV ERROR", err_no);
			goto fail_to_connect;
		}
	}
	num_connections++;
fail_to_connect:
	return;
}

void Test_Thread()
{
	while (true) {
		//Sleep(max(20, global_delay));
		Adjust_Number_Of_Client(); // 적당한 속도로 동접을 늘려야 한다.

		for (int i = 0; i < num_connections; ++i) {
			if (false == g_clients[i].connected) continue;
			if (g_clients[i].last_move_time + 1s > high_resolution_clock::now()) continue;
			g_clients[i].last_move_time = high_resolution_clock::now();
			cs_move_packet my_packet;
			my_packet.size = sizeof(my_packet);
			my_packet.type = static_cast<char>(packet_type::cs_move);
			switch (rand() % 4) {
			case 0: 
			{
				srand((unsigned int)time(NULL) + 300);
				double rand_x = rand() % 100;
				double rand_y = rand() % 100;
				double rand_z = rand() % 100;
				my_packet.direction = direction::right;
				my_packet.current.x = 0.0;
				my_packet.current.y = 0.0;
				my_packet.current.z = 0.0;
				my_packet.yaw = 0.0;
				my_packet.location.x = rand_x;
				my_packet.location.y = rand_y;
				my_packet.location.z = rand_z;
			} 	break;
			case 1: 
			{
				srand((unsigned int)time(NULL) + 38726);
				double rand_x = rand() % 100;
				double rand_y = rand() % 100;
				double rand_z = rand() % 100;
				my_packet.direction = direction::left;
				my_packet.current.x = 0.0;
				my_packet.current.y = 0.0;
				my_packet.current.z = 0.0;
				my_packet.yaw = 0.0;
				my_packet.location.x = rand_x;
				my_packet.location.y = rand_y;
				my_packet.location.z = rand_z;
			} 	break;
			case 2: 
			{
				srand((unsigned int)time(NULL) + 97234);
				double rand_x = rand() % 100;
				double rand_y = rand() % 100;
				double rand_z = rand() % 100;
				my_packet.direction = direction::right;
				my_packet.current.x = 0.0;
				my_packet.current.y = 0.0;
				my_packet.current.z = 0.0;
				my_packet.yaw = 0.0;
				my_packet.location.x = rand_x;
				my_packet.location.y = rand_y;
				my_packet.location.z = rand_z;
			} 	break;
			case 3: 
			{
				srand((unsigned int)time(NULL) + 7643);
				double rand_x = rand() % 100;
				double rand_y = rand() % 100;
				double rand_z = rand() % 100;
				my_packet.direction = direction::left;
				my_packet.current.x = 0.0;
				my_packet.current.y = 0.0;
				my_packet.current.z = 0.0;
				my_packet.yaw = 0.0;
				my_packet.location.x = rand_x;
				my_packet.location.y = rand_y;
				my_packet.location.z = rand_z;
			} 	break;
			}
			my_packet.move_time = static_cast<unsigned>(duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count());
			SendPacket(i, &my_packet);
		}
	}
}

void InitializeNetwork()
{
	for (auto& cl : g_clients) {
		cl.connected = false;
		cl.id = INVALID_ID;
	}

	for (auto& cl : client_map) cl = -1;
	num_connections = 0;
	last_connect_time = high_resolution_clock::now();

	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

	for (int i = 0; i < 6; ++i)
		worker_threads.push_back(new std::thread{ Worker_Thread });

	test_thread = thread{ Test_Thread };
}

void ShutdownNetwork()
{
	test_thread.join();
	for (auto pth : worker_threads) {
		pth->join();
		delete pth;
	}
}

void Do_Network()
{
	return;
}

void GetPointCloud(int* size, float** points)
{
	int index = 0;
	for (int i = 0; i < num_connections; ++i) // 연결이 active 한 것을 x, y 좌표를 전역변수에 넣어서 보내주기
		if (true == g_clients[i].connected) {
			point_cloud[index * 2] = static_cast<float>(g_clients[i].x);
			point_cloud[index * 2 + 1] = static_cast<float>(g_clients[i].y);
			index++;
		}

	*size = index;
	*points = point_cloud;
}

