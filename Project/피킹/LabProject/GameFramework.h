#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#include "Timer.h"
#include "Scene.h"
#include "Player.h"

class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();
	float MousePosX=0;
	float MousePosY=0;

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance = NULL;
	HWND						m_hWnd = NULL;

    bool						m_bActive = true;          

	CGameTimer					m_GameTimer;

	HDC							m_hDCFrameBuffer = NULL;
    HBITMAP						m_hBitmapFrameBuffer = NULL;
	XMFLOAT3					m_xmf3CameraPos;
	XMFLOAT4					m_xmf4PicPosition;
	XMFLOAT4					m_xmf4ReverseCoord;

	XMFLOAT4					m_xmf4ResultCoord;
	XMFLOAT4					m_xmf4PicPositionY;
	XMFLOAT4X4					m_xmf4ChangeViewInverse;


	XMFLOAT4X4					m_xmf4x4Projection;
	XMFLOAT4X4					m_xmf4x4ProjectionInverse;

	XMFLOAT4X4					m_xmf4x4View;
	XMFLOAT4X4					m_xmf4x4ViewInverse;
	XMFLOAT4X4					m_xmf4x4ViewProInverse;



	CPlayer						*m_pPlayer = NULL;

	CScene						*m_pScene = NULL;
	CCamera						*m_pCamera = NULL;
public:
public:
	
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();
	XMFLOAT4 MakeCoord(XMFLOAT4, XMFLOAT4X4);
	void BuildObjects();
	void ReleaseObjects();
	void ProcessInput();
	XMFLOAT4 CoordTransform(int x, int y);

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	POINT						m_ptOldCursorPos;

	_TCHAR						m_pszFrameRate[50];
};

