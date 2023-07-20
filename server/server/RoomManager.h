#pragma once
// 방을 관리하기 위한 클래스
// 방의 개수는 어떤게 효율적일지 생각해봐야 한다.
//
#define MAX_ROOM 50

#include "protocol.h"

enum class room_mode { null, normal, speed };

class RoomManager
{
private:
	wchar_t room_name[MAX_NAME] = {};
	int host_id;
	int team_id;
	bool use;

public:
	bool is_use();
	void use_room();
	void exit_room();

	void set_room_name(wchar_t* room_name);
	wchar_t* get_room_name();
	void set_host_id(int id);
	int get_host_id();
	void set_team_id(int id);
	int get_team_id();

	void create_room();

public:
	RoomManager() {};
	~RoomManager() {};
};

