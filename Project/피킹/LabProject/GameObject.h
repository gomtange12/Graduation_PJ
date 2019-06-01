#pragma once

#include "Mesh.h"
#include "Camera.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CBoss;

class CBullet;
class CGameObject
{
public:
	CGameObject(CMesh *pMesh);
	CGameObject();
	virtual ~CGameObject();

public:
	bool						m_bActive = true;
	bool						m_bIsBossObjectAlive = true;
	bool						m_bIsRedObject = false;
	CMesh						*m_pMesh = NULL;
    XMFLOAT4X4					m_xmf4x4World;    

	BoundingOrientedBox			m_xmOOBB;

	CGameObject					*m_pObjectCollided = NULL;

	DWORD						m_dwColor;

	XMFLOAT3					m_xmf3MovingDirection;
	XMFLOAT3					m_xmf3Velocity;
	int							m_iBulletCount = 20;
	int							m_iItemCount = 0;
	int							m_iEnemySelect = 0;
	//bool						m_bIsBossObject = false;
	float						m_fMakeObjectRushTime = 0;
	float						m_fMovingSpeed;
	float						m_fMovingRange;
	int							m_iBossHp = 50;
	CBoss						*m_pBoss = NULL;
	XMFLOAT3					m_xmf3RotationAxis;
	XMINT3						m_xmInitPosition;
	float						m_fRotationSpeed;
	CBullet						*m_pBossBullet = NULL;

public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void MoveStrafe(float fDistance=1.0f);
	void MoveUp(float fDistance=1.0f);
	void MoveForward(float fDistance=1.0f);
	void Move(XMFLOAT3& vDirection, float fSpeed);
	void MakeBossBullet(XMFLOAT3 pos);
	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);
	void InitObject(XMFLOAT3 pos, bool objecttype); //老馆 利 积己
	void MakeBossObject(XMFLOAT3 pos); // 焊胶 积己

	vector<CBullet*>			m_vecBossBullet;

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT3 GetMovingDirection() { return m_xmf3MovingDirection; };
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class CExplosiveObject : public CGameObject
{
public:
	CExplosiveObject();
	virtual ~CExplosiveObject();

	bool						m_bBlowingUp = false;

	XMFLOAT4X4					m_pxmf4x4Transforms[EXPLOSION_DEBRISES];
	float						m_fElapsedTimes = 0.0f;

	float						m_fDuration = 2.5f;
	float						m_fExplosionSpeed = 10.0f;
	float						m_fExplosionRotation = 720.0f;
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

public:
	static CMesh				*m_pExplosionMesh;
	static XMFLOAT3				m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
	static void PrepareExplosion();
};

class CWallsObject : public CGameObject
{
public:
	CWallsObject();
	virtual ~CWallsObject();

public:
	XMFLOAT4					m_pxmf4WallPlanes[6];
};

