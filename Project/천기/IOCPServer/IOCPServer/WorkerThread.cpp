#include "pch.h"
#include "Protocol.h"
#include "WorkerThread.h"
#include "PacketManager.h"
#include "IOCPServer.h"
#include "Player.h"
#include "ThreadManager.h"

WorkerThread::WorkerThread()
{
}

WorkerThread::~WorkerThread()
{
}
void WorkerThread::Init() 
{
	mythread = std::thread([&]() { WorkerThread::Proc(); });
}
void WorkerThread::Proc() 
{
	while (true)
	{
		DWORD iosize;
		unsigned long long key;
		stOverEx *over;

		int ret = GetQueuedCompletionStatus(IOCPSERVER->IOCP, &iosize, &key,
			reinterpret_cast<WSAOVERLAPPED **>(&over), INFINITE);
		// 에러처리
		if (0 == ret) {
			int err_no = GetLastError();
			if (64 == err_no)
				PACKETMANAGER->ClientDisconnect(key);
			else error_display("GQCS : ", err_no);
			continue;
		}
		//접속종료 처리
		if (0 == iosize) {
			PACKETMANAGER->ClientDisconnect(key);
			continue;
		}

		if (OP_RECV == over->m_todo) {
			// 패킷 재조립
			int rest_data = iosize;
			unsigned char *ptr = over->m_IOCPbuf;
			int packet_size = 0;
			if (0 != objectManager->GetPlayer(key)->m_prev_size)
				packet_size = objectManager->GetPlayer(key)->m_packet_buf[0];

			while (rest_data > 0) {
				if (0 == packet_size) packet_size = ptr[0];
				int need_size = packet_size - objectManager->GetPlayer(key)->m_prev_size;
				if (rest_data >= need_size) {
					// 패킷을 조립 가능
					memcpy(objectManager->GetPlayer(key)->m_packet_buf
						+ objectManager->GetPlayer(key)->m_prev_size,
						ptr, need_size);

					//매칭 관리
					if (objectManager->GetPlayer(key)->m_match == false) {
						objectManager->MatchProcess(key, objectManager->GetPlayer(key)->m_packet_buf);
					}
					else { //인게임
						objectManager->ProcessPacket(key, objectManager->GetPlayer(key)->m_packet_buf);
					}
					//
					ptr += need_size;
					rest_data -= need_size;
					packet_size = 0;
					objectManager->GetPlayer(key)->m_prev_size = 0;
				}
				else {
					// 패킷을 완성할 수 없으니 남은 데이터 저장
					memcpy(objectManager->GetPlayer(key)->m_packet_buf
						+ objectManager->GetPlayer(key)->m_prev_size,
						ptr, rest_data);
					ptr += rest_data;
					objectManager->GetPlayer(key)->m_prev_size += rest_data;
					rest_data = 0;
				}
			}
			THREADMANAGER->OverlappedRecv(key);
		}
		//상태 점검해야함
		else {
			if (OP_SEND == over->m_todo)
				// 받자 마자 지워줘서 이상한 값이 들어간다.
				delete over;
		}
	}
}
void WorkerThread::error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"에러 " << lpMsgBuf << std::endl;
	while (true);
	LocalFree(lpMsgBuf);
}