// TwitchTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock.h>


int main()
{
	WSADATA wsaData;
	int iResult;
	DWORD dwError;
	SOCKET sock;
	struct hostent *remoteHost;
	struct sockaddr_in server;
	char host_name[] = "irc.chat.twitch.tv";
	struct in_addr addr;


	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	printf("Calling gethostbyname with %s\n", host_name);
	remoteHost = gethostbyname(host_name);
	if (!remoteHost)
	{
		printf("Failled to get %s(s) host by name... Error: %d\n", host_name, WSAGetLastError());
		exit(1);
	}
	printf("Successfully retrieved host by name...\n");
	int i = 0;
	while (remoteHost->h_addr_list[i] != 0)
	{
		addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
		printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
	}
	char *ipaddr = inet_ntoa(addr);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("Socket == Invalid_Socket...\n");
		exit(1);
	}

	printf("Gathered All IP Adresses... Using %s\n", ipaddr);
	ZeroMemory(&server, sizeof(&server));
	server.sin_addr.s_addr = inet_addr(ipaddr);
	server.sin_family = AF_INET;
	server.sin_port = htons(6667);
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0)
	{
		printf("Connected to %s OR %s...\n", host_name, ipaddr);
		//Sleep(2000);
		//printf("Trying: PASS securityreasons\nNICK ircbot");

		send(sock, "PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\n", strlen("PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r") + 1, 0); //토큰?
		send(sock, "NICK blqblq\n", strlen("NICK blqblq") + 1, 0);
		send(sock, "USER blqblq\n", strlen("USER blqblq") + 1, 0);

		send(sock, "CAP REQ :twitch.tv/commands\n", strlen("CAP REQ :twitch.tv/commands") + 1, 0);
		send(sock, "CAP REQ :twitch.tv/membership\n", strlen("CAP REQ :twitch.tv/membership") + 1, 0);
		send(sock, "CAP REQ :twitch.tv/tags\n", strlen("CAP REQ :twitch.tv/tags") + 1, 0);

		send(sock, "JOIN 1k1000\n", strlen("JOIN 1k1000") + 1, 0);
		
		
		send(sock, "PRIVMSG 1k1000 :hi\n", strlen("PRIVMSG 1k1000 :hi") + 1, 0);
		int mret=0;  char rbuf[1024];
		rbuf[mret] = 0;
		while (true)
		{
			ZeroMemory(&rbuf, sizeof(rbuf));
			mret = recv(sock, rbuf, sizeof(rbuf), 0);
			if (!mret)
			{
				printf("Connection broke...\n");
				exit(1);
			}
			else
			{
				if (rbuf)
					printf("%s\n", rbuf);
			}
			Sleep(700);
			
			
			
		}
		
	}
	else
	{
		printf("Failed to connect!\n");
	}
	
}