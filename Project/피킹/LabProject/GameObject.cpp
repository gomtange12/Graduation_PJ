#include "StdAfx.h"
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

XMVECTOR RandomUnitVectorOnSphere()
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		//�����ϰ� ��Ѹ��� ���⺤��. x�� -1~1����, y�� -1~1���� z�� -1~1����
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)) return(XMVector3Normalize(v));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject() 
{ 
	m_pMesh = NULL; 
	m_xmf4x4World = Matrix4x4::Identity();
	m_dwColor = RGB(0, 0, 0);
	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh) : CGameObject()
{
	m_pMesh = pMesh; 
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z) 
{
	m_xmf4x4World._41 = x; 
	m_xmf4x4World._42 = y; 
	m_xmf4x4World._43 = z; 
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position) 
{ 
	m_xmf4x4World._41 = xmf3Position.x; 
	m_xmf4x4World._42 = xmf3Position.y; 
	m_xmf4x4World._43 = xmf3Position.z; 
}
void CGameObject::InitObject(XMFLOAT3 pos, bool isRedObject)
{
	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0,0,0), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetMesh(pObjectCubeMesh);

	//�Ϲ� ���� �� ��ġ���� 100m�տ� ����� ������ ��ġ�� ����
	m_xmInitPosition.x = rand() % 90 - 45;
	m_xmInitPosition.y = rand() % 90 - 45;

	SetPosition(rand() % 90 - 45, rand() % 90 - 45, pos.z + 100);
	
	SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));


	if (!isRedObject) //������ ���� �ƴ϶��
	{
		m_iEnemySelect = rand() % 3 + 1;
		//���� ���� ����: ��ũ, �ʷ�, �Ķ�
		if (m_iEnemySelect == 1) SetColor(RGB(170, 0, 120));
		else if (m_iEnemySelect == 2) SetColor(RGB(0, 0, 190));
		else if (m_iEnemySelect == 3) SetColor(RGB(0, 190, 0));
		m_bIsRedObject = false;
	}
	else
	{
		SetColor(RGB(255, 0, 0));
		m_bIsRedObject = true;

	}
	//���� ȸ��
	SetRotationSpeed(90.0f);
	SetMovingDirection(XMFLOAT3(Vector3::Subtract(pos, GetPosition())));

	//���� ������ �̵��ӵ��� �ٸ��� ����
	if (m_iEnemySelect == 1) SetMovingSpeed(3); 
	else if (m_iEnemySelect == 2) SetMovingSpeed(6);
	else if (m_iEnemySelect == 3) SetMovingSpeed(9);
}
void CGameObject::MakeBossBullet(XMFLOAT3 pos)
{
	//m_pBoss->m_vecBossBullet.emplace_back(new CBullet(m_pBoss->GetPosition(), GetRight(), GetUp(), GetLook(), m_xmf3Velocity, GetMovingDirection()));
}
void CGameObject::MakeBossObject(XMFLOAT3 pos)
{
	CCubeMesh *pObjectCubeMesh = new CCubeMesh(40.0f, 20.0f, 20.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(20.0f, 10.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetPosition(0, 0, 450);
	SetMesh(pObjectCubeMesh);
	SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetRotationSpeed(160.0f);
	//SetMovingDirection(XMFLOAT3(Vector3::Subtract(pos, GetPosition())));
	//SetMovingRange(50);
	if(GetPosition().z- pos.z<100) //�÷��̾ 100���� �̳��� �����ϸ� �÷��̾�� �����Ѵ�
		SetMovingSpeed(20);
	//SetColor(RGB(0, 0, 0));
	//cout << GetPosition().z << endl;
}
XMFLOAT3 CGameObject::GetPosition() 
{ 
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); 
}

XMFLOAT3 CGameObject::GetLook() 
{ 	
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp() 
{ 	
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{ 	
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fYaw, fPitch, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
	
	if (m_pMesh)
	{
		m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_pMesh)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		m_pMesh->Render(hDCFrameBuffer, m_xmf4x4World, pCamera);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallsObject::CWallsObject()
{
}

CWallsObject::~CWallsObject()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh *CExplosiveObject::m_pExplosionMesh = NULL;

CExplosiveObject::CExplosiveObject()
{
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp)
	{
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration)
		{
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
				m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
			}
			
			//m_ppObjects[i]->m_bActive = false;

		}
		else
		{
			m_bActive = false;
			m_bBlowingUp = false;
			m_fElapsedTimes = 0.0f;
		}
	}
	else
	{
		CGameObject::Animate(fElapsedTime);
	}
}

void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_bBlowingUp)
	{
		for (int i = 0; i < EXPLOSION_DEBRISES; i++)
		{
			if (m_pExplosionMesh)
			{
				HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
				HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
				m_pExplosionMesh->Render(hDCFrameBuffer, m_pxmf4x4Transforms[i], pCamera);
				::SelectObject(hDCFrameBuffer, hOldPen);
				::DeleteObject(hPen);
			}
		}
	}
	else
	{
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
}
