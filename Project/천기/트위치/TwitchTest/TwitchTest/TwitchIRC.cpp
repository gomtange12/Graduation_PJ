#include "pch.h"
#include "헤더.h"
#pragma execution_character_Set("utf-8")

void TwitchIRC::Init() 
{
	SetConsoleOutputCP(65001); //콘솔 인코딩

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	hostAdd = gethostbyname(hostName);

	int i = 0;
	while (hostAdd->h_addr_list[i] != 0)
	{
		ServerAddr.sin_addr.s_addr = *(u_long *)hostAdd->h_addr_list[i++];
	}
	char *ipaddr = inet_ntoa(ServerAddr.sin_addr);

	tw_sock = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(TW_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr(ipaddr);

	if (connect(tw_sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == 0)
	{
		printf("TwitchIRC Connected..! \n");
	
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
	int mret = 0;
	while (1) {
		DWORD dwTmp=0;
		
		//메세지 전송용
		
	/*	char text[1024] = { 0 };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		send(tw_sock, text, strlen(text), 0);
		if (strcmp(recv_buffer, "exit") == 0) {
			send(tw_sock, recv_buffer, (int)strlen(recv_buffer), 0);
			break;
		}*/
		
		mret = recv(tw_sock, (char *)recv_buffer, sizeof(MAX_BUFFER) - 1, 0);
		if (mret == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		response += string((char *)recv_buffer);
		fill_n(recv_buffer, sizeof(recv_buffer), NULL);
		if (response.size() > 1 && response[response.size() - 2] == '\r' && response[response.size() - 1] == '\n') {
			
				string name, message;
				stripMessage(response, name, message);
				cout << "Chat: " << name << ": " << message << endl;
		}
	}

}
void TwitchIRC::stripMessage(string incoming, string &username, string &message) {
	string cName = "#1k1000";
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
	send(tw_sock, "PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\r\n", strlen("PASS oauth:ccuwxr3mx5ks5q9pzv04x7k33rue9r\r") + 1, 0); //토큰?
	send(tw_sock, "NICK blqblq\r\n", strlen("NICK blqblq\r") + 1, 0);
	send(tw_sock, "USER blqblq\r\n", strlen("USER blqblq\r") + 1, 0);

	send(tw_sock, "CAP REQ :twitch.tv/commands\r\n", strlen("CAP REQ :twitch.tv/commands\r") + 1, 0);
	send(tw_sock, "CAP REQ :twitch.tv/membership\r\n", strlen("CAP REQ :twitch.tv/membership\r") + 1, 0);
	send(tw_sock, "CAP REQ :twitch.tv/tags\r\n", strlen("CAP REQ :twitch.tv/tags\r") + 1, 0);

	send(tw_sock, "JOIN #1k1000\r\n", strlen("JOIN #1k1000\r") + 1, 0);
}
//void TwitchIRC::setNonBlocking(const bool status) {
//	UL block = status;
//	S32 opt = ioctlsocket(tw_sock, FIONBIO, &block);
//	if (opt) {
//		cout << "tw_socket::setNonBlocking(): Failed with error " << WSAGetLastError() << endl;
//	}
//}
TwitchIRC::TwitchIRC()
{
}

TwitchIRC::~TwitchIRC()
{
	closesocket(tw_sock);
	WSACleanup();
}
