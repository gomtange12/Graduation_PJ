#pragma once

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

#include "Object.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
protected:
	XMFLOAT3					m_xmf3Position;// = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right;// = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up;// = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look;// = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float           			m_fMaxVelocityXZ = 0.0f;
	float           			m_fMaxVelocityY = 0.0f;
	float           			m_fFriction = 0.0f;
	bool						m_OnAacting{ FALSE };
	bool						m_isCrashMap{ false };
	LPVOID						m_pPlayerUpdatedContext = NULL;
	LPVOID						m_pCameraUpdatedContext = NULL;

	//CCamera						*m_pCamera = NULL;
	//이넘만들기
	
	
	std::shared_ptr<CCamera>	m_pCamera;
	bool m_AllowKey = false;
	
public:
	
	void SetCollimdeBox() {
		//if ( != nullptr)
		SetOOBB(GetPosition(), XMFLOAT3(0.5, 0.5, 1), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.f));
	};

	int m_PlayerState = IDLE;
	std::shared_ptr<CCamera> GetCamera() { return(m_pCamera); }
	virtual void SetCamera(std::shared_ptr<CCamera> pCamera) { m_pCamera = pCamera; }
	void SetYPosition(float ypos) { m_xmf3Position.y = ypos; }
	void SetAllowKey(bool key) { m_AllowKey = key; }
	bool GetAllowKey()const { return m_AllowKey; }
	CPlayer();
	CPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);

	virtual ~CPlayer();
	int GetPlayerState() { return m_PlayerState; }
	void SetPlayerState(PlayerState state) { m_PlayerState = state; }
	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }
	void SetLookVector(const XMFLOAT3& look) { m_xmf3Look = look; }
	void SetRightVector(const XMFLOAT3& right) { m_xmf3Right = right; }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void SetPosition(const XMFLOAT3& xmf3Position) { Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false); }

	void SetScale(XMFLOAT3& xmf3Scale) { m_xmf3Scale = xmf3Scale; }

	const XMFLOAT3& GetVelocity() const { return(m_xmf3Velocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	/*CCamera *GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }*/

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);
	bool IsPlayerCrashMap() { return m_isCrashMap; };
	void SetPlayCrashMap(bool isCrash);
	void Update(float fTimeElapsed);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed) { }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	//CCamera *OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	std::shared_ptr<CCamera>  OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual std::shared_ptr<CCamera> ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, std::shared_ptr<CCamera> pCamera = NULL);

	
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext=NULL);
	virtual ~CAirplanePlayer();

	CGameObject					*m_pMainRotorFrame = NULL;
	CGameObject					*m_pTailRotorFrame = NULL;

private:
	virtual void OnPrepareAnimate();
	virtual void Animate(float fTimeElapsed);

public:
	virtual std::shared_ptr<CCamera> ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
};

class CSoundCallbackHandler : public CAnimationCallbackHandler
{
public:
	CSoundCallbackHandler() { }
	~CSoundCallbackHandler() { }

public:
	virtual void HandleCallback(void *pCallbackData); 
};

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer() = default;
	CTerrainPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext=NULL);
	virtual ~CTerrainPlayer();

public:
	virtual std::shared_ptr<CCamera> ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	virtual void OnCameraUpdateCallback(float fTimeElapsed);
	//virtual void Animate(float fTimeElapsed);
	//virtual void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent);
};
class COtherPlayers : public CTerrainPlayer
{
public:
	COtherPlayers(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL); 
	virtual ~COtherPlayers(){}
public:

};


