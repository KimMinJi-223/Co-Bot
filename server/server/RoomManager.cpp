#include "RoomManager.h"

#include <iostream>

bool RoomManager::is_use()
{
	return use;
}

void RoomManager::use_room()
{
	use = true;
}

void RoomManager::exit_room()
{
	use = false;
	host_id = -1;
	team_id = -1;
	stage = -1;
	number_of_people = -1;
}

void RoomManager::set_room_name(wchar_t* room_name)
{
	wcscpy_s(this->room_name, MAX_ROOM, room_name);
}

wchar_t* RoomManager::get_room_name()
{
	return room_name;
}

void RoomManager::set_host_id(int id)
{
	host_id = id;
}

int RoomManager::get_host_id()
{
	return host_id;
}

void RoomManager::set_team_id(int id)
{
	team_id = id;
}

int RoomManager::get_team_id()
{
	return team_id;
}

void RoomManager::set_number_of_people(int number)
{
	number_of_people = number;
}

int RoomManager::get_number_of_people()
{
	return number_of_people;
}

void RoomManager::set_stage(int stage)
{
	this->stage = stage;
}

int RoomManager::get_stage()
{
	return stage;
}


