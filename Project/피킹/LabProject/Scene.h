#pragma once

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
class CBullet;
class CScene
{
public:
	CScene();
	virtual ~CScene();

	CPlayer						*m_pPlayer = NULL;

	CBullet						*m_pBullet = NULL;
	bool						m_bIsRedObject= false;

	int							m_nObjects = 0;
	float						m_fMakeObjectTime = 0;
	float						m_fMakeRedObjectTime = 0;
	float						m_fMakeObjectTRush = 0;
	float						m_MousePosX = 0;
	float						m_MousePosY = 0;
	
	CGameObject					**m_ppObjects = NULL;
	CGameObject					*m_pBossObject = NULL;
	CWallsObject				*m_pWallsObject = NULL;
	virtual void BuildObjects();
	virtual void ReleaseObjects();
	void OnMouseProcessing(WPARAM state, int x, int y);
	virtual void CheckObjectByObjectCollisions();
	virtual void CheckObjectByWallCollisions();
	virtual void CheckPlayerByObjectCollision();
	virtual void CheckObjectByBulletCollision ();
	virtual void CheckBossObjectByObjectCollision();
	virtual void CheckPlayerByBossBulletCollision();
	virtual void CheckPlayerByBossCollision();

	int CheckDistance(XMFLOAT3 pos, XMFLOAT3 checkpos);
	void MakeEnemy(bool isRedObject);
	void MakeBossEnemy(XMFLOAT3 pos);
	XMFLOAT4 CoordTransform(int, int);
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

