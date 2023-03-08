#include "ServerMain.h"

#include <MSWSock.h> // AcceptEx()
#include <vector>
#include <thread>
#include <iostream>
#include <array>
#include <mutex>

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

    //// DB 연동
    //sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenv); // 환경 핸들 할당
    //if (sqlret == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO) {
    //    sqlret = SQLSetEnvAttr(sqlenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0); // ODBC 버전 환경 속성 설정
    //    if (sqlret == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO) {
    //        sqlret = SQLAllocHandle(SQL_HANDLE_DBC, sqlenv, &sqldbc); // 연결 핸들 할당
    //        if (sqlret == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO) {
    //            SQLSetConnectAttr(sqldbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0); // login timeout 5초로 설정
    //            sqlret = SQLConnect(sqldbc, (SQLWCHAR*)L"COBOT_2023", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0); // 연결할 데이터 소스
    //            if (sqlret == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO) {
    //                sqlret = SQLAllocHandle(SQL_HANDLE_STMT, sqldbc, &sqlstmt); // 상태 핸들 할당
    //            }
    //        }
    //    }
    //}

    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        GetLastError();
        return false;
    }

    iocp_handle = CreateIOCPHandle();
    if (nullptr == iocp_handle)
    {
        GetLastError();
        return false;
    }

    server_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
    sockaddr_in server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT_NUM);

    if (SOCKET_ERROR == bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)))
    {
        GetLastError();
        return false;
    }

    if (SOCKET_ERROR == listen(server_sock, SOMAXCONN))
    {
        GetLastError();
        return false;
    }

    if (!AssociateSocketWithIOCP(server_sock, 7777))
    {
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
    {
       worker_threads.emplace_back(std::thread(&ServerMain::worker_thread, this));
    }

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

        if ((0 == num_bytes) && ((IO_RECV == over_ex->mode) || (IO_SEND == over_ex->mode)))
        {
            /*
                연결 끊기 코드 추가해야 함
            */

            if (IO_SEND == over_ex->mode)
            {
                delete over_ex;
            }

            continue;
        }

        switch (over_ex->mode)
        {
        case IO_ACCEPT:
        {
            int client_id = get_client_id();

            std::cout << client_id << "번째 ID를 가진 클라가 입장하였습니다." << std::endl;

            if (client_id != -1)
            {
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
                clients[client_id].x = -7700.f + static_cast<double>(client_id * 300);
                clients[client_id].y = 2180.f + static_cast<double>(client_id * 300);
                clients[client_id].z = 64.741069f; // +static_cast<double>(client_id * 300);
                clients[client_id].yaw = 0.f;

                // 접속을 다시 받기 위해서
                if (!AssociateSocketWithIOCP(client_sock, client_id))
                {
                    GetLastError();
                    continue;
                }

                clients[client_id].recv_packet();

                client_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

            }
            else
            {
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
                if (packet_size <= remain_data)
                {
                    process_packet(p, static_cast<int>(key));
                    p = p + packet_size;
                    remain_data = remain_data - packet_size;
                } else break;
            }

            clients[key].prev_remain = remain_data;

            if (remain_data > 0)
            {
                memcpy(over_ex->buffer, p, remain_data);
            }

            clients[key].recv_packet();

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
			//	//ring_buff->move_read_pos(p[0]);
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
            /*
                delete 해줘야 하는게 맞는데 왜 이거만 키면 오류가 나는지 모르겠다.

                <해결>
                할당하지도 않은 메모리 영역을 해제하려고 해서 생긴 오류였다.
                send_packet() 함수에서 new OVER_EX로 하여 오류를 해결하였다.
                근데 꼭 new/delete로 안해도 될것 같긴 하다.
            */
            delete over_ex;
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
    case static_cast<int>(type::cs_login):
    {
        cs_login_packet* pack = reinterpret_cast<cs_login_packet*>(packet);

        printf("%d client로부터 login packet을 받았습니다.\n", client_id);

        //wchar_t query_str[256];
        //wsprintf(query_str, L"SELECT * FROM user_table WHERE ")

        wcscpy_s(clients[client_id].name, MAX_LOGIN_LEN, pack->id);
        wprintf(L"%d client의 name: %s,\n", client_id, clients[client_id].name);
        {
            std::lock_guard<std::mutex> lock{ clients[client_id].state_lock };
            clients[client_id].state = state::ingame;
        }

        // matching
        // 팀원을 찾을 때까지 무한 대기
		for (int i{}; ; ++i)
		{
			if (MAX_USER == i) i = 0;
			if (state::ingame != clients[i].state || clients[i].id == client_id) continue;

            clients[client_id].match_lock.lock();
            if (-1 == clients[client_id].tm_id)
            {
                clients[i].match_lock.lock();
                if (-1 == clients[i].tm_id)
                {
                    clients[client_id].tm_id = clients[i].id;
                    clients[i].tm_id = client_id;
                    clients[i].match_lock.unlock();
                    clients[client_id].match_lock.unlock();
                    break;
                }
                else
                {
                    clients[i].match_lock.unlock();
                }
            } 
            else
            {
                clients[client_id].match_lock.unlock();
                break;
            }
		}

        // 상대방 좌표 기억하게 하기
        clients[client_id].tm_x = clients[clients[client_id].tm_id].x;
        clients[client_id].tm_y = clients[clients[client_id].tm_id].y;
        clients[client_id].tm_z = clients[clients[client_id].tm_id].z;
        clients[client_id].tm_yaw = clients[clients[client_id].tm_id].yaw;

        // 두 명 입장이 되어야 로그인이 됨
        clients[client_id].send_login_packet();
    } break;
    case static_cast<int>(type::cs_move):
    {
        cs_move_packet* pack = reinterpret_cast<cs_move_packet*>(packet);
        //printf("%d ID를 가진 클라이언트가 %f, %f, %f 로 이동하였습니다.\n", client_id, pack->x, pack->y, pack->z);
        clients[client_id].x = pack->x;
        clients[client_id].y = pack->y;
        clients[client_id].z = pack->z;
        clients[client_id].yaw = pack->yaw;
        clients[clients[client_id].tm_id].tm_x = pack->x;
        clients[clients[client_id].tm_id].tm_y = pack->y;
        clients[clients[client_id].tm_id].tm_z = pack->z;
        clients[clients[client_id].tm_id].tm_yaw = pack->yaw;
        // 움직인 것을 다른 클라들한테 보내주기
        // 23.2.11
        // 현재는 클라에서 움직임을 다 한 후 서버로 위치를 보내준다.
        // 혼자 하는 것이라면 상관이 없지만 멀티게임에서는 동시에 보여지는 것이 중요하다.
        // 이렇게 하면 다른 쪽에서는 느리게 보일 수 밖에 없다.
        // 누르는 순간 서버로 보내고 서버에서 동시에 보내주도록 해야 한다.
        // <현재 오류>
        // 클라 CPP에서 실행하면 클라 한명 당 2개의 접속이 되고 있다.
        // 따라서 접속도 안되어 있는 클라가 INGAME으로 표시되는 오류가 있다.
        // 이건 클라쪽에서 수정해야 할 듯 하다.
        clients[client_id].send_move_packet(client_id); // 움직인 클라한테 보내기
        clients[clients[client_id].tm_id].send_move_packet(client_id); // 상대 클라한테 보내기
    } break;
    //case static_cast<int>(type::cs_level2):
    //{
    //    cs_level2_packet* pack = reinterpret_cast<cs_level2_packet*>(packet);
    //    printf("%d 클라이언트가 level 2로 넘어갔습니다\n", client_id);
    //    // clients[client_id].x = 
    //} break;
    case static_cast<int>(type::cs_logout):
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
    }
}

