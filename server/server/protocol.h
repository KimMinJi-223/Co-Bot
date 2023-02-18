#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 2;
constexpr int MAX_LOGIN_LEN = 15;

enum class type { cs_login, sc_login, sc_add_player, cs_move, sc_move, cs_logout };

struct cs_login_packet {
	char size;
	char type;
	//wchar_t id[MAX_LOGIN_LEN];
	//wchar_t passward[MAX_LOGIN_LEN];
};

struct sc_login_packet {
	char size;
	char type;
	int id;
	double x;
	double y;
	double z;
	double yaw;
	double tm_x;
	double tm_y;
	double tm_z;
	double tm_yaw;
};

struct cs_move_packet {
	char size;
	char type;
	double x;
	double y;
	double z;
	double yaw;
};

struct sc_move_packet {
	char size;
	char type;
	int client_id;
	double x;
	double y;
	double z;
	double yaw;
};

struct cs_logout_packet {
	char size;
	char type;
};