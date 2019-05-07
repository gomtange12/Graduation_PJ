#pragma once

//공용 키값//
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20
#define DIR_JUMP					0x40
//

#define MAX_BUFFER		1024
#define SERVER_PORT		3500
#define MAX_USER		10

//
const int OP_RECV = 1;
const int OP_SEND = 2;
const int OP_MOVE = 3; //가속도 처리용
//

constexpr int SC_LOGIN_OK = 1;
constexpr int SC_PUT_PLAYER = 2;
constexpr int SC_REMOVE_PLAYER = 3;
constexpr int SC_MOVE_PLAYER = 4;
constexpr int SC_MATCHING_RESULT = 5;
constexpr int SC_SCENE = 6;
constexpr int SC_VECTOR_INFO = 7;
constexpr int SC_COLLISION = 8;
//
constexpr int CS_MATCHING_PLAYER = 1;
constexpr int CS_MOVE_STATE_INFO = 2;
constexpr int CS_POS_INFO = 3;
constexpr int CS_ROTE_STATE_INFO = 4;
#pragma pack (push, 1)
//////////////////////서버/////////////////////////
struct sc_packet_login_ok {
	BYTE size;
	BYTE type;
	char id;
	float posX;
	float posY;
	float posZ;
};
struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	char id;
	char x;
	char y;
};
struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	char id;
};

struct sc_packet_scene {
	BYTE size;
	BYTE type;
	WORD sceneNum;
	WORD roomNum;
	char ids; //나중에 여러플레이들 id로 뿌려줘야함
};
struct sc_packet_match_result {
	BYTE size;
	BYTE type;
	char result;
};

struct sc_packet_move {
	BYTE size;
	BYTE type;
	WORD id;
	bool velocity;
	float posX;
	float posY;
	float posZ;
}; 
struct sc_packet_vector {
	BYTE size;
	BYTE type;
	char id;
	float RposX;
	float RposY;
	float RposZ;
	float LposX;
	float LposY;
	float LposZ;
};
struct sc_packet_collision {
	BYTE size;
	BYTE type;
	bool check;
};
/////////////////////////클라//////////////////////
struct cs_packet_matching {
	BYTE size;
	BYTE type;
	char map;
	char avatar;
	char mod;
	//bool ready;
};

struct cs_packet_move_state {
	BYTE size;
	BYTE type;
	DWORD state;
	float time;
};
struct cs_packet_rote_state {
	BYTE size;
	BYTE type;
	float y;
};
struct cs_packet_pos{
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
};