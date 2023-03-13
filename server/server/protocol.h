#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 10;
constexpr int MAX_LOGIN_LEN = 15;

enum class packet_type { cs_login, sc_login, sc_add_player, cs_move, sc_move, sc_next_light, cs_logout };
enum class synch_type { button1, button2, button3 };

struct cs_login_packet {
	char	size;
	char	type;
	wchar_t id[MAX_LOGIN_LEN];
	wchar_t passward[MAX_LOGIN_LEN];
};

struct sc_login_packet {
	char	size;
	char	type;
	int		id;
	double	x;
	double	y;
	double	z;
	double	yaw;
	double	tm_x;
	double	tm_y;
	double	tm_z;
	double	tm_yaw;
};

struct cs_move_packet {
	char	size;
	char	type;
	double	x;
	double	y;
	double	z;
	double	yaw;
	unsigned int move_time;
};

struct sc_move_packet {
	char	size;
	char	type;
	int		client_id;
	double	x;
	double	y;
	double	z;
	double	yaw;
	unsigned int move_time;
};

struct sc_synch_packet {
	char	size;
	char	type;
	char	synch_type;
};

struct cs_logout_packet {
	char size;
	char type;
};