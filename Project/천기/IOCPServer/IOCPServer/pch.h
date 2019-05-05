﻿#ifndef PCH_H
#define PCH_H

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <winsock2.h>
#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <thread>


#include "SingleTone.h"

#define IOCPSERVER		IOCPServer::GET_INSTANCE()
#define PACKETMANAGER	PacketManager::GET_INSTANCE()
#define OBJMANAGER		ObjManager::GET_INSTANCE()
#define THREADMANAGER	ThreadManager::GET_INSTANCE()
#define ROOMMANAGER		RoomManager::GET_INSTANCE()

#define PERSONNEL 2


//using namespace std;
enum AVATARNumber {
	A, //임시명칭
	B
};
enum MAPNumber {
	PLAYGROUND,
	CONCERT
	
};
enum ModNumber {
	SOLO,
	DUO,
	SQUAD
};
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum SceneState {
	MENUSCENE = 0,
	INGAME = 1,
	GAMEOVER,
	LOADING
};
enum PlayerState {
	IDLE,
	RUN,
	JUMP,
	DEATH,
	STUN,
	HIT,
	JUMPROLL,
	RUN_JUMP_ATTAK,
	KICK,
	ATTACK,
	HAPPY,
	SAD,
	BACK_RUN
};

#endif //PCH_H
