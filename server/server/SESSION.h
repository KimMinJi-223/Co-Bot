#pragma once
#include "OBJECT.h"
#include "pch.h"

#include <mutex>

#include "RingBuffer.h"

enum class state { free, alloc, ingame };

class SESSION : public OBJECT
{
public:
	state		state;
	SOCKET		sock;
	int			id;				// 고유 id
	wchar_t		name[MAX_NAME];
	int			stage;
	
	int			room_id;

	vector_d3	location;
	vector_d3	current_left;
	vector_d3	current_right;
	float		time_left;
	float		time_right;

	double		yaw;
	RingBuffer	ring_buff;
	
	int			stage3_player_number;
	bool		mouse_left_click;

	bool		move_car;

	std::mutex	state_lock;
	std::mutex	match_lock;
	std::mutex	sock_lock;
	std::mutex	mouse_lock;

	OVER_EX		recv_over;

	// 레이턴시를 알기 위한 변수
	int			last_move_time;

	int			tm_id;	// 팀원 id
	vector_d3	tm_location;
	vector_d3	tm_current_left;
	vector_d3	tm_current_right;
	float		tm_time_left;
	float		tm_time_right;

	double		tm_yaw;

	int			tm_last_mvoe_time;

	// 패킷 재조립을 위한 부분 이거 나중에 꼭 수정해야 함
	int			prev_remain;
	int			prev_packet_size;
	char		prev_packet_buff[10000];

public:
	void recv_packet();
	void send_packet(char* packet);
	void send_signup_success_packet();
	void send_signup_fail_packet();
	void send_login_success_packet();
	void send_login_fail_packet();
	void send_create_room_ok(wchar_t* room_name, int room_mode);
	void send_game_start_packet(int stage);
	void send_show_room_list_packet(wchar_t* room_name, wchar_t* host_name, int room_id, int stage);
	void send_show_room_list_end_packet();
	void send_left_move_packet(int client_id);
	void send_right_move_packet(int client_id);
	void send_push_maze_button_packet(direction direction);
	void send_push_gear_button_packet(direction direction);
	void send_elevator_ok_packet();
	void send_forklift_button(packet_type type);
	void send_cobot_button(packet_type type);
	void send_board_color(int color, int client_id);
	void send_logout_packet();
	void send_move_car_packet(float direction, float acceleration);
	void send_stage3_enter_packet(int id, int tm_id);
	void send_cannon_yaw_packet(double value);
	void send_cannon_pitch_packet(double value);
	void send_cannon_click_packet(int click_id);
	void send_cannon_fire_packet();

public:
	SESSION();
	~SESSION();
};

