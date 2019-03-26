
#ifndef PCH_H
#define PCH_H

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <winsock2.h>
#include <iostream>
//
#include <vector>
#include <thread>
//
using namespace std;


#define MAX_BUFFER		1024
#define SERVER_PORT		3500
#define MAX_USER		10

#endif //PCH_H
