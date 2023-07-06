#include "ServerMain.h"

int main()
{
	ServerMain server;

	if (server.init()) // 초기화가 성공했다면 루프 돌기
	{
		server.server_main();
	}
}