#pragma once
#include "pch.h"
#include <concurrent_priority_queue.h>
// #include <sqlext.h>

class ServerMain
{
private:
	HANDLE	iocp_handle;
	SOCKET	server_sock, client_sock;
	OVER_EX overlapped;

	concurrency::concurrent_priority_queue<TIMER_EVENT> timer_queue;

	//SQLHENV		sqlenv;
	//SQLHDBC		sqldbc;
	//SQLHSTMT	sqlstmt = 0;
	//SQLRETURN	sqlret;

public:
	bool init();
	void server_main();

private:
	HANDLE CreateIOCPHandle();
	bool AssociateSocketWithIOCP(SOCKET sock, ULONG_PTR key);
	void worker_thread();
	int get_client_id();
	void process_packet(char* packet, int client_id);
	bool matching(int client_id);
	void set_team_position(int client_id);
	void do_timer_thread();

public:
	ServerMain();
	~ServerMain();
};

