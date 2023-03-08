#pragma once
#include "pch.h"
// #include <sqlext.h>

class ServerMain
{
private:
	HANDLE	iocp_handle;
	SOCKET	server_sock, client_sock;
	OVER_EX overlapped;

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

public:
	ServerMain();
	~ServerMain();
};

