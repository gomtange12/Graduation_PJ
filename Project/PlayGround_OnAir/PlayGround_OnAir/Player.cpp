//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Shader.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CNetWork.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPlayer
//int CPlayer::m_PlayeState = IDLE;

//CPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL)
//{
//	//m_pCamera = std::make_shared<CCamera>();
//	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);
//
//	//m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);
//	//m_pCamera->SetMode(THIRD_PERSON_CAMERA);
//	//m_pCamera->GenerateProjectionMatrix()
//	///m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
//
//	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
//	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
//
//	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	m_fMaxVelocityXZ = 0.0f;
//	m_fMaxVelocityY = 0.0f;
//	m_fFriction = 0.0f;
//
//	m_fPitch = 0.0f;
//	m_fRoll = 0.0f;
//	m_fYaw = 0.0f;
//
//	m_pPlayerUpdatedContext = NULL;
//	m_pCameraUpdatedContext = NULL;
//	m_PlayerState = PlayerState::IDLE;
//	CreateShaderVariables(pd3dDevice, pd3dCommandList);
//
//	SetPlayerUpdatedContext(pContext);
//	SetCameraUpdatedContext(pContext);
//}





CPlayer::CPlayer()
{

}

CPlayer::CPlayer(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
{
	//if(PLAYER->GetPlayer()!=NULL)
	m_pCamera = std::make_shared<CCamera>();
	//m_pCamera->Rotate(90, 0, 0);
	//m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);
	//11m_pCamera->SetMode(THIRD_PERSON_CAMERA);
	//m_pCamera->GenerateProjectionMatrix()
	///m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
	m_PlayerState = PlayerState::IDLE;

	if (this != nullptr)
	{
		CreateShaderVariables(pd3dDevice, pd3dCommandList);

		SetPlayerUpdatedContext(pContext);
		SetCameraUpdatedContext(pContext);
	}
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();

	//if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();
}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
		if (dwDirection & DIR_JUMP)
		{
			PLAYER->GetPlayer()->SetPlayerState(PlayerState::JUMP);
		}

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (this!=nullptr)
	{
		if (bUpdateVelocity)
		{
			m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
		}
		else
		{
			m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
			//if(m_pCamera!=nullptr)
			m_pCamera->Move(xmf3Shift);
		}
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y,z);
		if (y != 0.0f)
		{
			//CNETWORK->RotePkt(y);
			//서버연결시 주석
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
			//
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	//서버연결시 주석
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
	//
}


void CPlayer::SetPlayCrashMap(bool isCrash)
{
	m_isCrashMap = isCrash;
	
}

void CPlayer::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pPlayerUpdatedContext)
		OnPlayerUpdateCallback(fTimeElapsed);
	
	if (PLAYER->GetPlayer()!=nullptr)
	{
		//if (m_pCamera != nullptr)
		{
			DWORD nCurrentCameraMode = m_pCamera->GetMode();
			if (nCurrentCameraMode == THIRD_PERSON_CAMERA) {
			m_pCamera->Update(PLAYER->GetPlayer()->GetPosition(), fTimeElapsed);
			//m_pCamera->Update(PLAYER->GetOtherPlayer()->GetPosition(), fTimeElapsed);

			}
			if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
			if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(PLAYER->GetPlayer()->GetPosition());
			//m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);
			m_pCamera->RegenerateViewMatrix();
		}
	}

	//m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

	//m_pCamera->GenerateProjectionMatrix();
	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	//m_JumpPower = 0;
	switch (GetPlayerState())
	{
	/*default:
		SetTrackAnimationSet(0, IDLE);
		break;*/
	case IDLE:
	case RUN:
		//if (!m_OnAacting)
		//{
			SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
			m_OnAacting = FALSE;
		//}
		//SetTrackAnimationSet(0, RUN);
		//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
		//m_OnAacting = FALSE;
		break;
	case JUMP:
		m_OnAacting = TRUE;
		//float jumpTime = fTimeElapsed;
		//XMFLOAT3 jump{ 0,0,0 };
		//float newVel = -xmf3Velocity.y;
		//xmf3Velocity.y += m_xmf3Gravity.y * fTimeElapsed;
		////m_xmf3Position += 
		/*m_xmf3Position.y = m_xmf3Position.y + (m_xmf3Gravity.y * fTimeElapsed);
		cout << m_xmf3Position.y << endl;*/
		//m_newYpos = m_xmf3Position.y;
		//m_xmf3Position.y = m_xmf3Position.y + (m_xmf3Gravity.y* 0.5) * fTimeElapsed ;
		//m_xmf3Position.y += 7;
		/*m_newYpos = m_xmf3Position.y + 8;
		m_newYpos += (m_xmf3Gravity.y * 0.5* fTimeElapsed);
		m_xmf3Position.y = m_newYpos;*/
		m_JumpPower += (m_xmf3Gravity.y * 0.1* fTimeElapsed);
		m_xmf3Position.y += m_JumpPower * fTimeElapsed;
		if (m_JumpPower < 470)
			m_PlayerState = FALLING;

		//m_newYpos = 0;
		cout << m_JumpPower << endl;
		SetTrackAnimationSet(0, JUMP);
		break;
	case STUN:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, STUN);
		break;
	case JUMPROLL:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, JUMPROLL);
		break;
	case RUN_JUMP_ATTAK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, RUN_JUMP_ATTAK);
		break;
	case KICK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, KICK);
		break;
	case ATTACK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, ATTACK);
		break;
	case HAPPY:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, HAPPY);
		break;
	case SAD:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, SAD);
		break;
	case BACK_RUN:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, BACK_RUN);
		break;
	}
	//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
	//cout <<"점프 후"<< m_xmf3Position.y << endl;


	m_xmOOBB.Center = m_xmf3Position;
	m_xmOOBB.Center.y = m_xmf3Position.y + GetBoundingBox().Extents.y;
	//SetTrackAnimationSet(0, 2);

}

//CCamera *CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
std::shared_ptr<CCamera> CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)

{
	//CCamera *pNewCamera = NULL;
	std::shared_ptr<CCamera> pNewCamera = nullptr;

	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = std::make_shared<CFirstPersonCamera>(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = std::make_shared<CSpaceShipCamera>(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = std::make_shared<CThirdPersonCamera>(m_pCamera);
		//pNewCamera->Rotate(-90, 0, 0);

		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		
		//pNewCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		pNewCamera->SetTimeLag(0.25f);
		//pNewCamera->SetOffset(XMFLOAT3(0.0f, 350.0f, -80.0f));
		pNewCamera->SetOffset(XMFLOAT3(0.0f, 180.0f, -180.0f));
		//pNewCamera->SetOffset(XMFLOAT3(0.0f, 980.0f, -180.0f));

		if(PLAYER->GetPlayer()!=nullptr)
			pNewCamera->SetPosition(Vector3::Add(m_xmf3Position, PLAYER->GetPlayer()->GetCamera()->GetOffset()));
		//pNewCamera->Rotate(-90, 0, 0);
		pNewCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		pNewCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		pNewCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		//pNewCamera->Rotate(-90, 0, 0);
		//pNewCamera->Update();
		break;
	}	
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}
	/*else if ((nNewCameraMode == THIRD_PERSON_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
		m_xmf3Position.y = m_pCamera->GetPosition().y + 500;
	}*/
	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		//PLAYER->GetPlayer()->SetCamera(pNewCamera);
		//this->SetCamera(pNewCamera);
		//pNewCamera->SetPlayer(this);
	}

	//if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

std::shared_ptr<CCamera> CPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(2.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(2.5f);
		SetMaxVelocityY(40.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(100.5f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(40.0f);
		SetMaxVelocityY(40.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		//SetFriction(20.5f);
		//SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		//SetMaxVelocityXZ(25.5f);
		//SetMaxVelocityY(20.0f);
		//m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		//m_pCamera->SetTimeLag(0.25f);
		////m_pCamera->Rotate(0, -50, 0);
		//m_pCamera->SetOffset(XMFLOAT3(0.0f, 55.0f, -10.0f));
		//m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
		//m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		//m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		//m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		//break;
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 10.0f, -15.0f));
		m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void CPlayer::Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) CGameObject::Render(pd3dCommandList, pCamera);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
CAirplanePlayer::CAirplanePlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);

	CLoadedModelInfo *pModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Mi24.bin", NULL, false);
	SetChild(pModel->m_pModelRootObject);

	OnPrepareAnimate();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	if (pModel) delete pModel;
}

CAirplanePlayer::~CAirplanePlayer()
{
}

void CAirplanePlayer::OnPrepareAnimate()
{
	m_pMainRotorFrame = FindFrame("Top_Rotor");
	m_pTailRotorFrame = FindFrame("Tail_Rotor");
}

void CAirplanePlayer::Animate(float fTimeElapsed)
{
	if (m_pMainRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame->m_xmf4x4ToParent);
	}
	if (m_pTailRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(360.0f * 4.0f) * fTimeElapsed);
		m_pTailRotorFrame->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pTailRotorFrame->m_xmf4x4ToParent);
	}

	CPlayer::Animate(fTimeElapsed);
}

void CAirplanePlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();
}

std::shared_ptr<CCamera> CAirplanePlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			SetFriction(2.0f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(2.5f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case SPACESHIP_CAMERA:
			SetFriction(100.5f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(40.0f);
			SetMaxVelocityY(40.0f);
			m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case THIRD_PERSON_CAMERA:
			SetFriction(20.5f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(25.5f);
			SetMaxVelocityY(20.0f);
			m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.25f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 15.0f, -10.0f));
			m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		default:
			break;
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
#define _WITH_DEBUG_CALLBACK_DATA

void CSoundCallbackHandler::HandleCallback(void *pCallbackData)
{
   _TCHAR *pWavName = (_TCHAR *)pCallbackData; 
#ifdef _WITH_DEBUG_CALLBACK_DATA
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("%s\n"), pWavName);
	OutputDebugString(pstrDebug);
#endif
#ifdef _WITH_SOUND_RESOURCE
  // PlaySound(pWavName, ::ghAppInstance, SND_RESOURCE | SND_ASYNC);
#else
   //PlaySound(pWavName, NULL, SND_FILENAME | SND_ASYNC);
#endif
}

CTerrainPlayer::CTerrainPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
	: CPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext)
{
	/*m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;*/
	if (this != nullptr)
	{
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, 0x00);
		m_pCamera->SetLookAt(m_xmf3Position);
		//m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	}

	//m_ObjType = DYNAMIC;
	//if(CNETWORK->GetInstance()->)
	
	m_BoundScale = 1.5;
	CLoadedModelInfo *pPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/keytar_modi2.bin", NULL, true);

	SetChild(pPlayerModel->m_pModelRootObject, true);
	//int i = pPlayerModel->m_pModelRootObject->GetMeshType();
	//if(m_pMesh!=nullptr)
	//this->SetMesh(pPlayerModel->m_pModelRootObject->m_pMesh);
	
	m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pPlayerModel);

	m_pAnimationController = new CAnimationController(1, pPlayerModel->m_pAnimationSets);
	//SetOOBB(GetPosition(), XMFLOAT3(1,2,1),  XMFLOAT4(0, 0, 0, 1));
	m_pAnimationController->SetTrackAnimationSet(0, 0);

	m_pAnimationController->SetCallbackKeys(1, 3);
#ifdef _WITH_SOUND_RESOURCE
	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Footstep01"));
	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Footstep02"));
	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Footstep03"));
#else
	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Sound/Footstep01.wav"));
	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Sound/Footstep02.wav"));
	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Sound/Footstep03.wav"));
#endif
	CAnimationCallbackHandler *pAnimationCallbackHandler = new CSoundCallbackHandler();
	m_pAnimationController->SetAnimationCallbackHandler(1, pAnimationCallbackHandler);

	if (this != nullptr)
	{
		CreateShaderVariables(pd3dDevice, pd3dCommandList);
		SetPlayerUpdatedContext(pContext);
		SetCameraUpdatedContext(pContext);
	}
	//SetOOBB(GetPosition(), m_pMesh->GetAABBExtents(), XMFLOAT4(0, 0, 0, 1));
	//OBJECTMANAGER->AddGameObject(this, m_ObjType);
	//if (pPlayerModel) delete pPlayerModel;
	
}

CTerrainPlayer::~CTerrainPlayer()
{
}

std::shared_ptr<CCamera> CTerrainPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
		case FIRST_PERSON_CAMERA:
			SetFriction(250.0f);
			SetGravity(XMFLOAT3(0.0f, -400.0f, 0.0f));
			SetMaxVelocityXZ(300.0f);
			SetMaxVelocityY(400.0f);
			m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case SPACESHIP_CAMERA:
			SetFriction(125.0f);
			SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			SetMaxVelocityXZ(300.0f);
			SetMaxVelocityY(400.0f);
			m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.0f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case THIRD_PERSON_CAMERA:
			SetFriction(250.0f);
			SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
			SetMaxVelocityXZ(300.0f);
			SetMaxVelocityY(400.0f);
			m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.25f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 10.0f, -50.0f));
			m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		case MAP_CAMERA:
			SetFriction(250.0f);
			SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
			SetMaxVelocityXZ(300.0f);
			SetMaxVelocityY(400.0f);
			m_pCamera = OnChangeCamera(MAP_CAMERA, nCurrentCameraMode);
			m_pCamera->SetTimeLag(0.25f);
			m_pCamera->SetOffset(XMFLOAT3(0.0f, 10.0f, -50.0f));
			m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
			m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
			m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
			m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
			break;
		default:
			break;
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

void CTerrainPlayer::OnPlayerUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight{ 0 };// = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;

	/*int num = 0;
	switch (SCENEMANAGER->GetSceneType())
	{
	case PLAYGROUNDMAP:
		
		break;
	case CONCERTMAP:
		
		break;
	}*/
	if (PLAYER->GetPlayer()->IsPlayerCrashMap())
	{
		//if(PLAYER->GetPlayer()->GetPlayerState()== FALLING)
			fHeight = PLAYER->GetPlayer()->GetHeight();
	}
	else
	{
		fHeight = 0;
	}
	if (xmf3PlayerPosition.y < fHeight)
	{
		//if (FindFrame("LFootBone1")->GetPosition().y < fHeight)
		//	FindFrame("LFootBone1")->SetPosition(xmf3PlayerPosition);
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);	
	}



}

void CTerrainPlayer::OnCameraUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	//xmf3CameraPosition.y += 500;
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight{ 0 };// = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	//float boundHeight = 
	//fHeight = 255; //여기
	fHeight = 0;
	if (xmf3CameraPosition.y <= fHeight)

	xmf3CameraPosition.y = fHeight;
	m_pCamera->SetPosition(xmf3CameraPosition);
	
	if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA && m_pCamera != nullptr)
	{

		std::shared_ptr<CCamera> p3rdPersonCamera = m_pCamera;
		p3rdPersonCamera->SetLookAt(GetPosition());
	}
	
}
////////////////////////////
//void CTerrainPlayer::Animate(float fTimeElapsed)
//{
//	CGameObject::Animate(fTimeElapsed);
//}
//void CTerrainPlayer::UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent) {
//
// CGameObject::UpdateTransform(NULL);
//}

COtherPlayers::COtherPlayers(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, void * pContext)
	: CPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext)
{
	//차후에 리소스 관리 방식을 바꿔야한다
	/*m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;*/
	//m_BoundScale = 1;
	if (this != nullptr) //일단 카메라 쓰지 않는다
	{
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, 0x00);
		//m_pCamera->SetLookAt(m_xmf3Position);
		//m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	}

	//m_ObjType = DYNAMIC;
	CLoadedModelInfo *pPlayerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/guitarTest.bin", NULL, true);


	SetChild(pPlayerModel->m_pModelRootObject, true);
	m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pPlayerModel);

	m_pAnimationController = new CAnimationController(1, pPlayerModel->m_pAnimationSets);
	m_pAnimationController->SetTrackAnimationSet(0, 0);

	m_pAnimationController->SetCallbackKeys(1, 3);
#ifdef _WITH_SOUND_RESOURCE
	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Footstep01"));
	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Footstep02"));
	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Footstep03"));
#else
	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Sound/Footstep01.wav"));
	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Sound/Footstep02.wav"));
	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Sound/Footstep03.wav"));
#endif
	CAnimationCallbackHandler *pAnimationCallbackHandler = new CSoundCallbackHandler();
	m_pAnimationController->SetAnimationCallbackHandler(1, pAnimationCallbackHandler);

	if (this != nullptr)
	{
		CreateShaderVariables(pd3dDevice, pd3dCommandList);
		SetPlayerUpdatedContext(pContext);
		SetCameraUpdatedContext(pContext);
	}
	//SetOOBB(GetPosition(), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.f));
	//OBJECTMANAGER->AddGameObject(this, m_ObjType);

	//if (pPlayerModel) delete pPlayerModel;
	
}

void COtherPlayers::OnPlayerUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight{ 0 };// = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;

	fHeight = 20;

	if (xmf3PlayerPosition.y < fHeight)
	{
		//if (FindFrame("LFootBone1")->GetPosition().y < fHeight)
		//	FindFrame("LFootBone1")->SetPosition(xmf3PlayerPosition);
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void COtherPlayers::OnCameraUpdateCallback(float fTimeElapsed)
{
	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	//XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	//XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	////xmf3CameraPosition.y += 500;
	//int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	//bool bReverseQuad = ((z % 2) != 0);
	//float fHeight{ 0 };// = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	////float boundHeight = 
	//fHeight = 255; //여기
	//if (xmf3CameraPosition.y <= fHeight)

	//	xmf3CameraPosition.y = fHeight;
	//m_pCamera->SetPosition(xmf3CameraPosition);
	//
}

void COtherPlayers::Update(float fTimeElapsed)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	if (m_pPlayerUpdatedContext)
		OnPlayerUpdateCallback(fTimeElapsed);

	if (PLAYER->GetOtherPlayer() != nullptr)
	{

			//m_pCamera = ChangeCamera(/*SPACESHIP_CAMERA*/THIRD_PERSON_CAMERA, 0.0f);
		m_pCamera->RegenerateViewMatrix();
		
	}

	//m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

	//m_pCamera->GenerateProjectionMatrix();
	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

	switch (GetPlayerState())
	{
		/*default:
			SetTrackAnimationSet(0, IDLE);
			break;*/
	case IDLE:
	case RUN:
		//if (!m_OnAacting)
		//{
		SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
		m_OnAacting = FALSE;
		//}
		//SetTrackAnimationSet(0, RUN);
		//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
		//m_OnAacting = FALSE;
		break;
	case JUMP:
		m_OnAacting = TRUE;
		m_newYpos = m_xmf3Position.y + 9;
		m_newYpos += (m_xmf3Gravity.y * 0.5)* fTimeElapsed;
		m_xmf3Position.y = m_newYpos;
		m_newYpos = 0;
		SetTrackAnimationSet(0, JUMP);
		//m_xmf3Position.y += (9.8) * fTimeElapsed + fTimeElapsed * m_xmf3Velocity.y;
		//cout << m_xmf3Position.y << endl; 
		break;
	case STUN:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, STUN);
		break;
	case JUMPROLL:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, JUMPROLL);
		break;
	case RUN_JUMP_ATTAK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, RUN_JUMP_ATTAK);
		break;
	case KICK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, KICK);
		break;
	case ATTACK:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, ATTACK);
		break;
	case HAPPY:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, HAPPY);
		break;
	case SAD:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, SAD);
		break;
	case BACK_RUN:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, BACK_RUN);
		break;
	}
	//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);

	m_xmOOBB.Center = m_xmf3Position;
}
