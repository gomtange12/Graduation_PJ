#pragma once


class Player;
class Packetmanager : public SingleTone<Packetmanager>{
public:
	
	Packetmanager();
	~Packetmanager();

	void SendPacket(int id, void *packet);
	void LoginPacking(int id);
	void PutPlayerPacking(int id);
	void PosPacking(int id, int x, int y);
};
