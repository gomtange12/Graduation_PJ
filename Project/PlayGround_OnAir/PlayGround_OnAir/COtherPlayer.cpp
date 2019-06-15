#include "stdafx.h"
#include "Player.h"
#include "COtherPlayer.h"
#include "CObjectManager.h"

COtherPlayers::COtherPlayers(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature, E_CHARACTERTYPE type, void * pContext)
	: CPlayer(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pContext)
{
	//차후에 리소스 관리 방식을 바꿔야한다
	//m_xmf3Position = XMFLOAT3(440.0f, 50, 1745);
	//m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//
	//m_xmf3Scale = XMFLOAT3(40, 20, 40);
	//m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//m_fMaxVelocityXZ = 0.0f;
	//m_fMaxVelocityY = 0.0f;
	//m_fFriction = 0.0f;
	//
	//m_fPitch = 0.0f;
	//m_fRoll = 0.0f;
	//m_fYaw = 0.0f;
	//Rotate(0, 90, 0);
	//m_BoundScale = 1;
	if (this != nullptr) //일단 카메라 쓰지 않는다
	{
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, 0x00);
		m_pCamera->SetLookAt(m_xmf3Position);
		m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	}

	//m_ObjType = DYNAMIC;
	CLoadedModelInfo* pPlayerModel = OBJECTMANAGER->GetPlayerResource(type);

	SetChild(pPlayerModel->m_pModelRootObject, true);
	m_pSkinningBoneTransforms = new CSkinningBoneTransforms(pd3dDevice, pd3dCommandList, pPlayerModel);

	m_pAnimationController = new CAnimationController(1, pPlayerModel->m_pAnimationSets);
	m_pAnimationController->SetTrackAnimationSet(0, 0);

	m_pAnimationController->SetCallbackKeys(2, 3);
	//#ifdef _WITH_SOUND_RESOURCE
	//	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Footstep01"));
	//	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Footstep02"));
	//	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Footstep03"));
	//#else
	//	m_pAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Sound/Footstep01.wav"));
	//	m_pAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Sound/Footstep02.wav"));
	//	m_pAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Sound/Footstep03.wav"));
	//#endif
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

	if (pPlayerModel) delete pPlayerModel;

}

void COtherPlayers::OnPlayerUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight{ 0 };// = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;

	//if (PLAYER->GetOtherPlayer()->IsPlayerCrashMap())
	//{
	//	//if(PLAYER->GetPlayer()->GetPlayerState()== FALLING)
	//	fHeight = PLAYER->GetOtherPlayer()->GetHeight();
	//}
	//else {
		fHeight = 10;
	//}
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void COtherPlayers::OnCameraUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	//xmf3CameraPosition.y += 500;
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight{ 0 };// = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	//float boundHeight = 
	fHeight = 0; //여기
	if (xmf3CameraPosition.y <= fHeight)

	xmf3CameraPosition.y = fHeight;
	m_pCamera->SetPosition(xmf3CameraPosition);

}

void COtherPlayers::Update(float fTimeElapsed)
{
	//if (PLAYER->GetOtherPlayer()->IsPlayerCrashMap() == false) {
	//	if (PLAYER->GetOtherPlayer()->GetCollisionState() == true)
	//	{
	//		PLAYER->GetOtherPlayer()->SetPosition(Vector3::Add(PLAYER->GetOtherPlayer()->GetPosition(), PLAYER->GetOtherPlayer()->GetLookVector(), -6.f));
	//
	//		PLAYER->GetOtherPlayer()->SetCollisionState(false);
	//
	//	}
	//}

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

	//if (PLAYER->GetOtherPlayerMap().size() > 0)
	//{
	//	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	//	for (int i = 0; i < PLAYER->GetOtherPlayerMap().size(); ++i)
	//	{
	//
	//		if (nCurrentCameraMode == THIRD_PERSON_CAMERA) {
	//			m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	//			//m_pCamera->Update(PLAYER->GetOtherPlayer()->GetPosition(), fTimeElapsed);
	//
	//		}
	//		if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
	//		if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_xmf3Position);
	//		//m_pCamera = ChangeCamera(/SPACESHIP_CAMERA/THIRD_PERSON_CAMERA, 0.0f);
	//	}
	//	//m_pCamera->RegenerateViewMatrix();
	//
	//
	//
	//}

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
		m_OnAacting = FALSE;
		SetTrackAnimationSet(0, IDLE);
		break;
	case RUN:
		//if (!m_OnAacting)
		//{
			//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
		m_OnAacting = FALSE;
		//}
		SetTrackAnimationSet(0, RUN);
		//SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);
		//m_OnAacting = FALSE;
		break;
	case JUMP:
		m_JumpPower += (m_xmf3Gravity.y * 0.1* fTimeElapsed);
		m_xmf3Position.y += m_JumpPower * fTimeElapsed;
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
	case ATTACK_3:
		m_OnAacting = TRUE;
		SetTrackAnimationSet(0, ATTACK_3);
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
	m_xmOOBB.Center.y = m_xmf3Position.y + GetBoundingBox().Extents.y;
}
