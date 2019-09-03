#include "stdafx.h"
#include "CSoundManager.h"


CSoundManager::CSoundManager()
{
	
	//Initialize();
}


CSoundManager::~CSoundManager()
{
	system->release();
	system->close();
}

void CSoundManager::LoadSound()
{
	result = system->createSound("Sound/MENU.wav", FMOD_LOOP_NORMAL, nullptr, &sound[MENU_SOUND]);
	ErrorCheck(result);
	result = system->createSound("Sound/INGAME.wav", FMOD_LOOP_NORMAL, nullptr, &sound[INGAME_SOUND]);
	ErrorCheck(result);
	result = system->createSound("Sound/PIANO.wav", FMOD_LOOP_OFF, nullptr, &sound[SKILL_SOUND]);
	ErrorCheck(result);
	result = system->createSound("Sound/basicHit.mp3", FMOD_LOOP_OFF, nullptr, &sound[ATTACK_SOUND]);
	ErrorCheck(result);
	
	sound[MENU_SOUND]->setMusicChannelVolume(MENU_SOUND, 0.3);
	sound[INGAME_SOUND]->setMusicChannelVolume(INGAME_SOUND, 0.3);
	sound[SKILL_SOUND]->setMusicChannelVolume(SKILL_SOUND, 0.3);


}

void CSoundManager::Initialize()
{
	result = System_Create(&system);
	ErrorCheck(result);
	result = system->init(30, FMOD_INIT_NORMAL, nullptr);
	ErrorCheck(result);
}

void CSoundManager::playSound(int type, int chNum)
{
	system->update();
	result = system->playSound(sound[type], 0, false, &channel[chNum]);
	ErrorCheck(result);
}

void CSoundManager::StopSound(int chNum)
{
	channel[chNum]->stop();
}

void CSoundManager::ErrorCheck(FMOD_RESULT _r)
{
	if (_r != FMOD_OK) {
		TCHAR str[256] = { 0 };
		MultiByteToWideChar(CP_ACP, NULL, FMOD_ErrorString(result), -1, str, 256);
		MessageBox(NULL, str, L"Sound Error", MB_OK);
	}
}
