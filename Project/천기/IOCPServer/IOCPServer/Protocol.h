#pragma once

#define MAX_BUFFER		1024
#define SERVER_PORT		3500
#define MAX_USER		10

const int OP_RECV = 1;
const int OP_SEND = 2;


////=확인용=/////
constexpr int WORLD_WIDTH = 8;
constexpr int WORLD_HEIGHT = 8;
////=확인용=/////


constexpr int SC_LOGIN_OK = 1;
constexpr int SC_PUT_PLAYER = 2;
constexpr int SC_REMOVE_PLAYER = 3;
constexpr int SC_MOVE_PLAYER = 4;
constexpr int SC_MATCHING_PLAYER = 5;
constexpr int SC_MATCHING_RESULT = 6;
constexpr int SC_SCENE = 7;

#pragma pack (push, 1)
//////////////////////packet/////////////////////////
struct sc_packet_pos {//SC_POSITION_INFO 3 
	BYTE size;
	BYTE type;
	WORD id;
	WORD X_POS;
	WORD Y_POS;
	//float X_POS;
};
struct sc_packet_login_ok {
	BYTE size;
	BYTE type;
	char id;
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
struct sc_packet_matching {
	BYTE size;
	BYTE type;
	char map;
	char avatar;
	char mod;
	//bool ready;
};
struct sc_packet_scene {
	BYTE size;
	BYTE type;
	WORD sceneNum;
	WORD roomNum;
};
struct sc_packet_match_result {
	BYTE size;
	BYTE type;
	char result;
};
