#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 10;

enum class type { cs_login, cs_move, sc_move, cs_level2 };

struct cs_login_packet {
	char size;
	char type;
};

struct cs_move_packet {
	char size;
	char type;
	double x;
	double y;
	double z;
};

struct sc_move_packet {
	char size;
	char type;
	int client_id;
	double x;
	double y;
	double z;
};

struct cs_level2_packet {
	char size;
	char type;
	int client_id;
};