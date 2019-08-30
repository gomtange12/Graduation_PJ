#pragma once
#include "fmod.hpp"
#include <fmod_errors.h>

class CSoundManager : public CSingleTonBase<CSoundManager>
{
	FMOD::System* system = nullptr;
	FMOD::Sound* sound[13]; // ��Ÿ ��ų ���� ���� �޴� 1 ���� �޴�2 ����
	FMOD::Channel* channel[30];
	FMOD_RESULT result;
public:
	CSoundManager();
	~CSoundManager();
	void LoadSound();
	void Initialize();
	void playSound(int type, int chNum);
	void StopSound(int chNum);
	void ErrorCheck(FMOD_RESULT _r);

};

