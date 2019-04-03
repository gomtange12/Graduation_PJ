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
//
#include <vector>
#include <thread>
#include "SingleTone.h"

#define IOCPSERVER		IOCPServer::GET_INSTANCE()
#define OBJMANAGER		ObjManager::GET_INSTANCE()
#define THREADMANAGER	ThreadManager::GET_INSTANCE()

//

using namespace std;


#define MAX_BUFFER		1024
#define SERVER_PORT		3500
#define MAX_USER		10


enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
#endif //PCH_H
