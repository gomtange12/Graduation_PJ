//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"
#define GET_X_LPARAM(lp)		((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)		((int)(short)HIWORD(lp))

CGameFramework::CGameFramework()
{
	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	BuildFrameBuffer();

	BuildObjects();

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, (rcClient.right - rcClient.left) + 1, (rcClient.bottom - rcClient.top) + 1);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight, m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		cout << GetCursorPos(&m_ptOldCursorPos) << endl;
		MousePosX = LOWORD(lParam);
		MousePosY = HIWORD(lParam);
		m_pScene->OnMouseProcessing(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		

		m_pScene->CoordTransform(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
XMFLOAT4 CGameFramework::MakeCoord(XMFLOAT4 vec, XMFLOAT4X4 mat)
{

	XMFLOAT4 f4PicPosition;
	f4PicPosition.x = vec.x * mat._11 + vec.x * mat._21 + vec.x * mat._31 + vec.x * mat._41;
	f4PicPosition.y = vec.y * mat._12 + vec.y * mat._22 + vec.y * mat._32 + vec.y * mat._42;
	f4PicPosition.z = vec.z * mat._13 + vec.z * mat._23 + vec.z * mat._33 + vec.z * mat._43;
	f4PicPosition.w = vec.w * mat._14 + vec.w * mat._24 + vec.w * mat._34 + vec.w * mat._44;

	return f4PicPosition;
}

XMFLOAT4 CGameFramework::CoordTransform(int x, int y)
{
	/*double x1 = x / CLIENT_WIDTH;
	double y1 = y / CLIENT_HEIGHT;
	cout << "2: " << x1 << ", " << y1 << endl;
	float transcoord_x = x * 2 - 1;
	
	float transcoord_y = y * 2 - 1;
	cout << "3: " << transcoord_x << ", " << transcoord_y << endl;*/

	//return XMFLOAT4(transcoord_x, transcoord_y, 1, 0);
	
	m_xmf3CameraPos = m_pCamera->GetPosition();
	//카메라의 위치를 받아온다
	m_xmf4x4Projection = m_pCamera->GetMtxProjection(); //투영행렬을 받아온다
	m_xmf4x4ViewInverse = Matrix4x4::Inverse(m_pCamera->GetViewMatrix());//뷰행렬을 받아온다

	m_xmf4x4ProjectionInverse = Matrix4x4::Inverse(m_pCamera->GetMtxProjection()); //투영행렬의 역행렬을 만든다.
	m_xmf4x4ViewProInverse = Matrix4x4::Inverse(m_pCamera->GetViewProject()); // 뷰행렬의 역행렬을 만든다.

	m_xmf4PicPosition = MakeCoord(m_xmf4PicPosition, m_xmf4x4ViewInverse); // 포지션과 뷰행렬역행렬을 곱해준다.
	m_xmf4ReverseCoord = MakeCoord(m_xmf4PicPosition, m_xmf4x4ProjectionInverse); 
	// 그 포지션 좌표에 투영행렬역행렬을 곱해준다. 
	//MakeCoord함수는 float4벡터와 4x4행렬을 곱해주는 함수를 만든 것이다. 원래 있는 인라인 함수를 활용하고자 했으나
	//*연산자가 오버로딩 되어있지 않아서 직접 만들었다. 

	m_xmf4ReverseCoord.x = (((2.0f*MousePosX) / CLIENT_WIDTH) - 1) / m_xmf4x4Projection._11; //투영변환
	m_xmf4ReverseCoord.y = -(((2.0f*MousePosX) / CLIENT_WIDTH) - 1) / m_xmf4x4Projection._22;
	m_xmf4ReverseCoord.z = 1.0;
	m_xmf4ReverseCoord.w = 0;
	//투영변환은 x,y좌표만 필요하기 때문에 x,y만 받아서 사용했다.MousePosX는 플레이어가 찍은 화면의 좌표이다.
	/*m_xmf4PicPosition.x =  m_xmf4PicPosition.x * m_xmf4x4ViewInverse._11 + m_xmf4PicPosition.x * m_xmf4x4ViewInverse._21+ m_xmf4PicPosition.x * m_xmf4x4ViewInverse._31 + m_xmf4PicPosition.x * m_xmf4x4ViewInverse._41;
	m_xmf4PicPosition.y = m_xmf4PicPosition.y * m_xmf4x4ViewInverse._12 + m_xmf4PicPosition.y * m_xmf4x4ViewInverse._22 + m_xmf4PicPosition.y * m_xmf4x4ViewInverse._32 + m_xmf4PicPosition.y * m_xmf4x4ViewInverse._42;
	m_xmf4PicPosition.z = m_xmf4PicPosition.z * m_xmf4x4ViewInverse._13 + m_xmf4PicPosition.z * m_xmf4x4ViewInverse._23 + m_xmf4PicPosition.z * m_xmf4x4ViewInverse._33 + m_xmf4PicPosition.z * m_xmf4x4ViewInverse._43;
	m_xmf4PicPosition.w = m_xmf4PicPosition.w * m_xmf4x4ViewInverse._14 + m_xmf4PicPosition.w * m_xmf4x4ViewInverse._24 + m_xmf4PicPosition.w * m_xmf4x4ViewInverse._34 + m_xmf4PicPosition.w * m_xmf4x4ViewInverse._44;
*/

	 m_xmf4ReverseCoord= MakeCoord(m_xmf4ReverseCoord, m_xmf4x4ViewProInverse);
	 //뷰행렬의 역행렬과 위에서 만든 벡터를 곱해주어 리턴한다.
	return m_xmf4ReverseCoord;

}
void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		default:
			m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::BuildObjects()
{
	CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);
	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, -490.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
	m_pPlayer->m_pMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3, 3, 3), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//m_pPlayer->m_pMesh->SetOOBB

	m_pScene = new CScene();
	m_pScene->m_pPlayer = m_pPlayer;
	m_pScene->BuildObjects();

}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene)
	{
		m_pScene->ReleaseObjects();
		delete m_pScene;
	}

	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
	if (m_hWnd) DestroyWindow(m_hWnd);
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;
	
	if (GetKeyboardState(pKeyBuffer))
	{
		if (KEYDOWN('W'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));

			dwDirection |= DIR_FORWARD;
		}
		if (KEYDOWN('S'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));

			dwDirection |= DIR_BACKWARD;
		}

		if (KEYDOWN('A'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));

			dwDirection |= DIR_LEFT;

		}
		if (KEYDOWN('D'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));
			dwDirection |= DIR_RIGHT;

		}
		if (KEYDOWN('Q'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));
			dwDirection |= DIR_UP;

		}
		if (KEYDOWN('E'))
		{
			//m_pPlayer->m_xmOOBB.Transform(m_pPlayer->m_xmOOBB, XMLoadFloat4x4(&m_pPlayer->m_xmf4x4World));

			dwDirection |= DIR_DOWN;

		}

	}
	if (RI_MOUSE_RIGHT_BUTTON_DOWN)
	{
		//m_pScene->m_MousePosX = MousePosX;
		//m_pScene->m_MousePosY = MousePosY;

	}


	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			else
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		}
		if (dwDirection) m_pPlayer->Move(dwDirection, 0.15f);
	}
	//m_pScene->
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::FrameAdvance()
{
	if (!m_bActive) return;

	m_GameTimer.Tick(0.0f);

	ProcessInput();
	//m_pScene->m_MousePosX = m_xmf4ReverseCoord;
	m_pScene->Animate(m_GameTimer.GetTimeElapsed());

	ClearFrameBuffer(RGB(255, 255, 255));

	m_pScene->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);
	m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}


