#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 10;
constexpr int MAX_NAME = 15;

enum class packet_type {
	cs_signup,

	sc_signup_success,
	sc_signup_fail,

	cs_login,

	sc_login_success,
	sc_login_fail,

	cs_create_room,
	sc_create_room_ok,

	cs_show_room_list,

	cs_enter,
	cs_move,
	cs_logout,

	cs_push_button_Forklift_red,
	cs_push_button_Forklift_green,
	cs_push_button_Forklift_blue,
	cs_push_button_Forklift_black,

	sc_push_button_Forklift_red,
	sc_push_button_Forklift_green,
	sc_push_button_Forklift_blue,
	sc_push_button_Forklift_black,

	cs_push_button_cobot_red,
	cs_push_button_cobot_green,
	cs_push_button_cobot_blue,
	cs_push_button_cobot_black,

	sc_push_button_cobot_red,
	sc_push_button_cobot_green,
	sc_push_button_cobot_blue,
	sc_push_button_cobot_black,

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

	cs_start_time_color,
	sc_board_color,

	sc_logout,

	cs_stage3_enter,
	sc_stage3_enter,

	cs_car_direction,
	sc_car_direction,

	cs_cannon,
	sc_cannon_yaw,
	sc_cannon_pitch,

	cs_cannon_click,
	sc_cannon_click,
	sc_cannon_fire,
};
enum class direction { forward, back, left, right };
enum class color { red, green, blue, black };
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

struct cs_signup_packet {
	char	size;
	char	type;
	wchar_t id[MAX_NAME];
	wchar_t pw[MAX_NAME];\
};

struct sc_signup_success_packet {
	char size;
	char type;
};

struct sc_signup_fail_packet {
	char size;
	char type;
};

struct cs_login_packet {
	char	size;
	char	type;
	wchar_t id[MAX_NAME];
	wchar_t passward[MAX_NAME];
};

struct sc_login_success_packet {
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

struct sc_login_fail_packet {
	char size;
	char type;
};

struct cs_create_room_packet {
	char	size;
	char	type;
	int		room_mode;
	wchar_t room_name[MAX_NAME];
};

struct sc_create_room_ok_packet {
	char	size;
	char	type;
	int		host_id;
	int		room_mode;
	wchar_t room_name[MAX_NAME];
};

struct cs_show_room_list_packet {
	char size;
	char type;
	int room_mode;
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

struct cs_stage2_last_gate_packet
{
	char size;
	char type;
};

struct sc_board_color_packet
{
	char size;
	char type;
	int color;
	int id;
};

struct sc_logout_packet
{
	char size;
	char type;
};

struct cs_car_direction_packet
{
	char size;
	char type;
	bool direction;
};

struct sc_car_direction_packet
{
	char	size;
	char	type;
	float	direction;
	float	acceleration;
};

struct cs_stage3_enter_packet
{
	char size;
	char type;
};

struct sc_stage3_enter_packet
{
	char	size;
	char	type;
	int		player_number;
};

struct cs_cannon_packet
{
	char	size;
	char	type;
	double	cannon_value;
};

struct sc_cannon_yaw_packet 
{
	char	size;
	char	type;
	float	yaw;
};

struct sc_cannon_pitch_packet
{
	char	size;
	char	type;
	float	pitch;
};

struct cs_cannon_click_packet
{
	char size;
	char type;
};

struct sc_cannon_click_packet
{
	char	size;
	char	type;
	int		click_id;
};

struct sc_cannon_fire_packet
{
	char size;
	char type;
};