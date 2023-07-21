#pragma once
// 방을 관리하기 위한 클래스
// 방의 개수는 어떤게 효율적일지 생각해봐야 한다.
//
#define MAX_ROOM 50

#include "protocol.h"

enum class room_mode { normal, speed, null };

class RoomManager
{
private:
	bool use;
	wchar_t room_name[MAX_NAME] = {};
	int host_id;
	int team_id;
	int stage;
	int number_of_people;

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
	void set_number_of_people(int number);
	int get_number_of_people();
	void set_stage(int stage);
	int get_stage();

public:
	RoomManager() 
		: use(false)
		, host_id(-1)
		, team_id(-1)
		, stage(-1) 
		, number_of_people(0) {};
	~RoomManager() {};
};

