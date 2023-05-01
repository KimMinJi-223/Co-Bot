#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 10;
constexpr int MAX_LOGIN_LEN = 15;

enum class packet_type {
	cs_login,
	cs_enter,
	cs_move,
	cs_logout,

	cs_push_button_Forklift_red,
	cs_push_button_Forklift_green,
	cs_push_button_Forklift_blue,
	cs_push_button_Forklift_black,

	cs_push_button_cobot_red,
	cs_push_button_cobot_green,
	cs_push_button_cobot_blue,
	cs_push_button_cobot_black,

	cs_push_button_maze_forward,
	cs_push_button_maze_back,
	cs_push_button_maze_left,
	cs_push_button_maze_right,

	cs_push_button_gear_forward,
	cs_push_button_gear_back,
	cs_push_button_gear_left,
	cs_push_button_gear_right,

	sc_login, 
	sc_add_player,
	sc_move,
	sc_next_light,

	sc_push_button_maze_forward,
	sc_push_button_maze_back,
	sc_push_button_maze_left,
	sc_push_button_maze_right,

	sc_push_button_gear_forward,
	sc_push_button_gear_back,
	sc_push_button_gear_left,
	sc_push_button_gear_right,

	cs_elevator,
	sc_elevator,
};
enum class direction { forward, back, left, right };
enum class synch_type { button1, button2, button3 };
enum class button_type { s2_maze_forward, s2_maze_back, s2_maze_right, s2_maze_left };
enum class endbutton_type { s2_maze_forward, s2_maze_back, s2_maze_right, s2_maze_left };


struct vector_d3 {
	double x;
	double y;
	double z;

public:
	vector_d3() : x(-1.f), y(-1.f), z(-1.f)
	{}

	vector_d3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

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

struct cs_enter_packet {
	char size;
	char type;
};

struct cs_move_packet {
	char		size;
	char		type;
	float		time;
	direction	direction;
	double		yaw;
	vector_d3	location;
	vector_d3	current;
};

//struct cs_left_move_packet {
//	char size;
//	char type;
//	double current_left_x;
//	double current_left_y;
//	double current_left_z;
//	double location_x;
//	double location_y;
//	double location_z;
//	float time_left;
//};

struct sc_move_packet {
	char		size;
	char		type;
	int			client_id;
	float		time;
	direction	direction;
	double		yaw;
	vector_d3	location;
	vector_d3	current;
};

//struct sc_move_packet {
//	char	size;
//	char	type;
//	int		client_id;
//	double	x;
//	double	y;
//	double	z;
//	double	yaw;
//	unsigned int move_time;
//};

struct sc_synch_packet {
	char	size;
	char	type;
	char	synch_type;
};

struct cs_logout_packet {
	char size;
	char type;
};

struct cs_button_packet {
	char size;
	char type;
};

struct sc_button_packet {
	char size;
	char type;
};

struct cs_elevator_packet
{
	char size;
	char type;
	int elevator_number;
};

struct sc_elevator_packet
{
	char size;
	char type;
};