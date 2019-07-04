#pragma once

//#define FRAME_BUFFER_WIDTH		1280
//#define FRAME_BUFFER_HEIGHT		720
#define FRAME_BUFFER_WIDTH		520
#define FRAME_BUFFER_HEIGHT		320

#include "Timer.h"
#include "Player.h"
#include "Scene.h"
#define	_WITH_DIRECT2D_IMAGE_EFFECT

class CGameFramework //: public CSingleTonBase<CGameFramework>
{
public:
	CGameFramework();
	~CGameFramework();

	CGameFramework*			GetCGameFramework() { return this; }
	void SetCamera(const shared_ptr<CCamera>& camera) { m_pCamera = camera; };

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void ChangeSwapChainState();

    void BuildObjects();
    void ReleaseObjects();

    void ProcessInput();
    void AnimateObjects();
    void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();
	DWORD dwDirection = 0;
	DWORD otherPlayerDirection = 0;

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool						m_ready = false;
#ifdef _WITH_DIRECT2D
	void CreateDirect2DDevice();
#endif

private:

	D3D12_VIEWPORT				m_d3dViewport;
	D3D12_RECT					m_d3dScissorRect;
	HINSTANCE					m_hInstance;
	HWND						m_hWnd; 
	int							m_nWndClientWidth;
	int							m_nWndClientHeight;
        
	IDXGIFactory4				*m_pdxgiFactory = NULL;
	IDXGISwapChain3				*m_pdxgiSwapChain = NULL;
	ID3D12Device				*m_pd3dDevice = NULL;

	bool						m_bMsaa4xEnable = false;
	UINT						m_nMsaa4xQualityLevels = 0;

	static const UINT			m_nSwapChainBuffers = 2;
	UINT						m_nSwapChainBufferIndex;

	ID3D12Resource				*m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap		*m_pd3dRtvDescriptorHeap = NULL;
	UINT						m_nRtvDescriptorIncrementSize;

	ID3D12Resource				*m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap		*m_pd3dDsvDescriptorHeap = NULL;
	UINT						m_nDsvDescriptorIncrementSize;

	ID3D12CommandAllocator		*m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue			*m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList = NULL;

	ID3D12Fence					*m_pd3dFence = NULL;
	UINT64						m_nFenceValues[m_nSwapChainBuffers];
	HANDLE						m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug					*m_pd3dDebugController;
#endif

	CGameTimer					m_GameTimer;

	CScene						*m_pScene = NULL;
	//CPlayer						*m_pPlayer = NULL;
	shared_ptr<CCamera>					m_pCamera = NULL;

	POINT						m_ptOldCursorPos;
	POINT						m_ptRightOldCursorPos;
	POINT						m_LeftCursorPos;

	//COtherPlayers				*pOtherPlayer = nullptr;

	_TCHAR						m_pszFrameRate[70];

#ifdef _WITH_DIRECT2D
	ID3D11On12Device			*m_pd3d11On12Device = NULL;
	ID3D11DeviceContext			*m_pd3d11DeviceContext = NULL;
	ID2D1Factory3				*m_pd2dFactory = NULL;
	IDWriteFactory				*m_pdWriteFactory = NULL;
	ID2D1Device2				*m_pd2dDevice = NULL;
	ID2D1DeviceContext2			*m_pd2dDeviceContext = NULL;

	ID3D11Resource				*m_ppd3d11WrappedBackBuffers[m_nSwapChainBuffers];
	ID2D1Bitmap1				*m_ppd2dRenderTargets[m_nSwapChainBuffers];

	ID2D1SolidColorBrush		*m_pd2dbrBackground = NULL;
	ID2D1SolidColorBrush		*m_pd2dbrBorder = NULL;
	IDWriteTextFormat			*m_pdwFont = NULL;
	IDWriteTextLayout			*m_pdwTextLayout = NULL;
	ID2D1SolidColorBrush		*m_pd2dbrText = NULL;

#ifdef _WITH_DIRECT2D_IMAGE_EFFECT
	IWICImagingFactory			*m_pwicImagingFactory = NULL;
	ID2D1Effect					*m_pd2dfxBitmapSource = NULL;
	ID2D1Effect					*m_pd2dfxGaussianBlur = NULL;
	ID2D1DrawingStateBlock1		*m_pd2dsbDrawingState = NULL;
	IWICFormatConverter			*m_pwicFormatConverter = NULL;
#endif
#endif

};

