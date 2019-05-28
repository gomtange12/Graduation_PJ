#include "pch.h"
#include "헤더.h"
void TwitchIRC::Init() {
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
		ServerAddr.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
		printf("\tIP Address #%d: %s\n", i, inet_ntoa(ServerAddr.sin_addr));
	}
	char *ipaddr = inet_ntoa(ServerAddr.sin_addr);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("Socket == Invalid_Socket...\n");
		exit(1);
	}

	printf("Gathered All IP Adresses... Using %s\n", ipaddr);
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(6667);
	ServerAddr.sin_addr.s_addr = inet_addr(ipaddr);

	if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == 0)
	{
		printf("Connected..! \n");
	
		InitSend();

		Run();
	}
	else
	{
		printf("Failed to connect!\n");
	}

}

void TwitchIRC::Run() 
{
	//send(sock, "PRIVMSG #1k1000 :hello world\n", strlen("PRIVMSG #1k1000 :hello world") + 1, 0);
	int mret = 0;
	while (1) {
		DWORD dwTmp=0;
		
		//메세지 전송용
		
	/*	char text[1024] = { 0 };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		send(sock, text, strlen(text), 0);
		if (strcmp(recv_buffer, "exit") == 0) {
			send(sock, recv_buffer, (int)strlen(recv_buffer), 0);
			break;
		}*/
		
		mret = recv(sock, recv_buffer, sizeof(recv_buffer) - 1, 0);
		if (mret == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		
		//recv_buffer[mret] = '\0';
		response += string((char *)recv_buffer);
		fill_n(recv_buffer, sizeof(recv_buffer), NULL);;
		//Check if the server sent it all in one go
		/*if (response.size() > 1 && response[response.size() - 2] == '\r' && response[response.size() - 1] == '\n') {
		
		}*/
		if (response.size() > 1 && response[response.size() - 2] == '\r' && response[response.size() - 1] == '\n') {
			
				string name, message;
				//Strip the username and their message
				stripMessage(response, name, message);
				//Push to log / console
				cout << "Chat: " << name << ": " << message << endl;
			
			
		}
		//if (response.find("PRIVMSG") != string::npos) {
		//	string name, message;
		//	//Strip the username and their message
		//	stripMessage(response, name, message);
		//	//Push to log / console
		//	cout << "Chat: " << name << ": " << message << endl;
		//}
		//else
		//{

			
		//}

		//printf("Receive Message : %s", recv_buffer);
	}






	/*while (true)
	{
		ZeroMemory(&recv_buffer, sizeof(recv_buffer));
		int mret = recv(sock, recv_buffer, sizeof(recv_buffer), 0);
		if (!mret)
		{
			printf("Connection broke...\n");
			exit(1);
		}
		else
		{
			if (recv_buffer)
				printf("%s\n", recv_buffer);
		}
	}*/
	//recv_buffer[mret] = '\0';

}
void TwitchIRC::stripMessage(string incoming, string &username, string &message) {
	string cName = "1k1000";
	SIZE_T nameBegin = incoming.find("display-name=") + 13;
	SIZE_T nameEnd = incoming.find(";", nameBegin);
	SIZE_T messageStart = incoming.find(cName + " :") + cName.size() + 2;
	//The correct format is :NAME!, test here
	username = incoming.substr(nameBegin, (nameEnd - nameBegin));
	//Fetch the message content
	if (messageStart != string::npos) {
		for (SIZE_T i = messageStart; i < incoming.size(); i++) {
			message.push_back(incoming[i]);
		}
	}
}
void TwitchIRC::InitSend() {
	send(sock, "PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\n", strlen("PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r") + 1, 0); //토큰?
	send(sock, "NICK blqblq\n", strlen("NICK blqblq") + 1, 0);
	send(sock, "USER blqblq\n", strlen("USER blqblq") + 1, 0);

	send(sock, "CAP REQ :twitch.tv/commands\n", strlen("CAP REQ :twitch.tv/commands") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/membership\n", strlen("CAP REQ :twitch.tv/membership") + 1, 0);
	send(sock, "CAP REQ :twitch.tv/tags\n", strlen("CAP REQ :twitch.tv/tags") + 1, 0);

	send(sock, "JOIN 1k1000\n", strlen("JOIN 1k1000") + 1, 0);
}
TwitchIRC::TwitchIRC()
{
}

TwitchIRC::~TwitchIRC()
{
	closesocket(sock);
	WSACleanup();
}
