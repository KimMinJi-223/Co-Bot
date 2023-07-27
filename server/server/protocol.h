#pragma once

constexpr int PORT_NUM = 7000;
constexpr int BUF_SIZE = 2048;

constexpr int MAX_USER = 1000;
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

	cs_delete_room,
	sc_delete_room,

	cs_enter_room,
	sc_enter_room_fail,

	sc_game_start,

	cs_show_room_list,
	sc_show_room_list,
	sc_show_room_list_end,

	cs_enter,
	sc_enter,

	cs_esc,
	sc_esc,

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

	sc_car_push_down,
	sc_car_push_up,

	cs_car_location,
	sc_car_location,

	cs_car_rotation_yaw,
	sc_car_rotation_yaw,

	cs_cannon,
	sc_cannon_yaw,
	sc_cannon_pitch,

	cs_cannon_click,
	sc_cannon_click,
	sc_cannon_fire,

	cs_select_bridge_widget,
	sc_select_bridge_widget,
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
	wchar_t pw[MAX_NAME];
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
	wchar_t id[MAX_NAME]; // 스트레스 테스트 떄문에 바꿈
	wchar_t passward[MAX_NAME];
	// char	id[MAX_NAME];
};

struct sc_login_success_packet {
	char	size;
	char	type;
	int		id;
	int		stage;
	//double	x;
	//double	y;
	//double	z;
	//double	yaw;
	//double	tm_x;
	//double	tm_y;
	//double	tm_z;
	//double	tm_yaw;
};

struct sc_login_fail_packet {
	char size;
	char type;
};

struct cs_create_room_packet {
	char	size;
	char	type;
	int		room_id;
	int		stage;
	wchar_t room_name[MAX_NAME];
};

struct cs_delete_room_packet {
	char	size;
	char	type;
	int		room_id;
};

struct sc_delete_room_packet {
	char size;
	char type;
};

struct sc_create_room_ok_packet {
	char	size;
	char	type;
	int		room_id;
	int		host_id;
	//int		stage;
	wchar_t room_name[MAX_NAME];
};

struct cs_show_room_list_packet {
	char size;
	char type;
};

struct sc_show_room_list_packet {
	char	size;
	char	type;
	wchar_t room_name[MAX_NAME];
	wchar_t host_name[MAX_NAME];
	int		room_id;
	int		stage;
};

struct sc_show_room_list_end_packet {
	char size;
	char type;
};

struct cs_enter_room_packet {
	char	size;
	char	type;
	int		room_id;
};

struct sc_enter_room_fail_packet {
	char size;
	char type;
};

struct sc_game_start_packet {
	char	size;
	char	type;
	int		stage;
};

struct cs_enter_packet {
	char size;
	char type;
};

struct sc_enter_packet {
	char	size;
	char	type;
	int		id;
};

struct cs_esc_packet {
	char	size;
	char	type;
};

struct sc_esc_packet {
	char	size;
	char	type;
	int		stage;
};

struct cs_move_packet {
	char		size;
	char		type;
	float		time;
	direction	direction;
	double		yaw;
	vector_d3	location;
	vector_d3	current;
	//unsigned	move_time; // 스트레스 테스트를 위하여
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
	//unsigned	move_time; // 스트레스 테스트를 위하여
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

struct sc_car_push_down_packet {
	char	size;
	char	type;
	int		player_number;
};

struct sc_car_push_up_packet {
	char	size;
	char	type;
	int		player_number;
};

struct cs_car_location_packet {
	char		size;
	char		type;
	vector_d3	car_location;
};

struct sc_car_location_packet {
	char		size;
	char		type;
	vector_d3	car_location;
};

struct cs_car_rotation_yaw_packet {
	char	size;
	char	type;
	float	yaw;
};

struct sc_car_rotation_yaw_packet {
	char	size;
	char	type;
	float	yaw;
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

struct cs_select_bridge_widget_packet {
	char	size;
	char	type;
	int		index;
};

struct sc_select_bridge_widget_packcet {
	char	size;
	char	type;
	int		index;
};