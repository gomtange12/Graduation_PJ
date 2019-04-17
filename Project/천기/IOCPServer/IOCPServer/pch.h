#ifndef PCH_H
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
#include <d3d12.h>
#include <DirectXMath.h>

#include "SingleTone.h"

#define IOCPSERVER		IOCPServer::GET_INSTANCE()
#define PACKETMANAGER	PacketManager::GET_INSTANCE()
#define OBJMANAGER		ObjManager::GET_INSTANCE()
#define THREADMANAGER	ThreadManager::GET_INSTANCE()

//using namespace std;
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum SceneState {
	LOADING,
	MENUSCENE,
	INGAME,
	GAMEOVER
};
#endif //PCH_H
