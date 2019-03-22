/*
## 소켓 서버 : 1 v n - overlapped callback
1. socket()            : 소켓생성
2. bind()            : 소켓설정
3. listen()            : 수신대기열생성
4. accept()            : 연결대기
5. read()&write()
	WIN recv()&send    : 데이터 읽고쓰기
6. close()
	WIN closesocket    : 소켓종료
*/

#include "pch.h"

#define MAX_BUFFER        1024
#define SERVER_PORT        3500
const int OP_RECV = 1;
const int OP_SEND = 2;
struct stOverEx {
	WSAOVERLAPPED m_wsaOver;
	WSABUF m_wsaBuf;
	unsigned char m_IOCPbuf[MAX_BUFFER]; // IOCP send/recv 버퍼
	unsigned char	m_todo;
};
struct ClientInfo {
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;
};
struct ChessClient {
	ClientInfo m_ClientInfo;
	wchar_t m_name[20];
	BYTE m_x;
	BYTE m_y;
	bool m_connected;
	int m_id;
};

ChessClient g_clients[10];
struct move_packet
{
	unsigned char size;
	unsigned char type;
};

//void CALLBACK callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
HANDLE	g_hIOCP;

void InitNetwork() 
{
	// Winsock Start - windock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		exit(1);
	}
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}
void accept_thread() {
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		exit(1);
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;

	while (1)
	{

		clientSocket = WSAAccept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			exit(1);
		}
		int id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (false == g_clients[i].m_connected) {
				id = i;
				break;
			}

		if (-1 == id) {
			printf("FULL \n");
			closesocket(clientSocket);
			continue;
		}
		//여기에
		//패킷을만듬 - 패킷에 데이터를 저장->타입변환해서 버퍼에 넣어줌 ->
		//WSASend(클라소켓, 저장된 버퍼, 1(버퍼갯수), NULL(보낸데이터크기), 0(동작옵션), 오버랩, 0)

		DWORD flags = 0;
		g_clients[id].m_id = id;
		g_clients[id].m_ClientInfo.m_socket = clientSocket;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_todo = OP_RECV;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaBuf.buf = (CHAR*)g_clients[0].m_ClientInfo.m_RecvOverEx.m_IOCPbuf;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaBuf.len = sizeof(g_clients[0].m_ClientInfo.m_RecvOverEx.m_IOCPbuf);
		g_clients[id].m_ClientInfo.m_prev_size = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[0].m_ClientInfo.m_socket), g_hIOCP, 0, 0);
		ZeroMemory(&g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
		printf("여기1");
		if (WSARecv(g_clients[id].m_ClientInfo.m_socket, &g_clients[0].m_ClientInfo.m_RecvOverEx.m_wsaBuf, 1, NULL, &flags, &(g_clients[0].m_ClientInfo.m_RecvOverEx.m_wsaOver), NULL) == SOCKET_ERROR)
		{

			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - IO pending Failure\n");
			}
		}
		else {
			printf("Non Overlapped Recv return.\n");

		}
		printf("여기2");
	}
}
void worker_thread() {

}

int main()
{
	const int NUM_THREADS = 4;
	std::vector <std::thread> worker_threads;

	InitNetwork();

	for (auto i = 0; i < NUM_THREADS; ++i)
		worker_threads.push_back(std::thread{ worker_thread });
	std::thread accept_th{ accept_thread };

	for (auto &wth : worker_threads) wth.join();
	accept_th.join();
	printf("여기3");
	// Winsock End
	WSACleanup();

	return 0;
}

//void CALLBACK callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
//{
//	struct SOCKETINFO *socketInfo;
//	DWORD sendBytes = 0;
//	DWORD receiveBytes = 0;
//	DWORD flags = 0;
//
//	socketInfo = (struct SOCKETINFO *)overlapped;
//	memset(&(socketInfo->overlapped), 0x00, sizeof(WSAOVERLAPPED));
//
//	if (dataBytes == 0)
//	{
//		closesocket(socketInfo->socket);
//		free(socketInfo);
//		return;
//	}
//
//	if (socketInfo->receiveBytes == 0)
//	{
//		// WSARecv(최초 대기에 대한)의 콜백일 경우
//		socketInfo->receiveBytes = dataBytes;
//		socketInfo->sendBytes = 0;
//		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
//		socketInfo->dataBuffer.len = socketInfo->receiveBytes;
//
//		printf("TRACE - Receive message : %s (%d bytes)\n", socketInfo->messageBuffer, dataBytes);
//
//		if (WSASend(socketInfo->socket, &(socketInfo->dataBuffer), 1, &sendBytes, 0, &(socketInfo->overlapped), callback) == SOCKET_ERROR)
//		{
//			if (WSAGetLastError() != WSA_IO_PENDING)
//			{
//				printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
//			}
//		}
//	}
//	else
//	{
//		// WSASend(응답에 대한)의 콜백일 경우
//		socketInfo->sendBytes += dataBytes;
//		socketInfo->receiveBytes = 0;
//		socketInfo->dataBuffer.len = MAX_BUFFER;
//		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
//
//		printf("TRACE - Send message : %s (%d bytes)\n", socketInfo->messageBuffer, dataBytes);
//
//		if (WSARecv(socketInfo->socket, &socketInfo->dataBuffer, 1, &receiveBytes, &flags, &(socketInfo->overlapped), callback) == SOCKET_ERROR)
//		{
//			if (WSAGetLastError() != WSA_IO_PENDING)
//			{
//				printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
//			}
//		}
//	}
//}