//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"
#include "CSceneManager.h"
#include "CMenuScene.h"
#include "CIngameScene.h"
#include "CNetWork.h"

CGameFramework::CGameFramework()
{
	m_pdxgiFactory = NULL;
	m_pdxgiSwapChain = NULL;
	m_pd3dDevice = NULL;

	for (int i = 0; i < m_nSwapChainBuffers; i++) m_ppd3dSwapChainBackBuffers[i] = NULL;
	m_nSwapChainBufferIndex = 0;

	m_pd3dCommandAllocator = NULL;
	m_pd3dCommandQueue = NULL;
	m_pd3dCommandList = NULL;

	m_pd3dRtvDescriptorHeap = NULL;
	m_pd3dDsvDescriptorHeap = NULL;

	m_nRtvDescriptorIncrementSize = 0;
	m_nDsvDescriptorIncrementSize = 0;

	m_hFenceEvent = NULL;
	m_pd3dFence = NULL;
	for (int i = 0; i < m_nSwapChainBuffers; i++)
	{
		m_nFenceValues[i] = 0;
#ifdef _WITH_DIRECT2D
		m_ppd3d11WrappedBackBuffers[i] = NULL;
		m_ppd2dRenderTargets[i] = NULL;
#endif
	}
	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
	//m_pPlayer = NULL;

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
#ifdef _WITH_DIRECT2D
	CreateDirect2DDevice();
#endif
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();

	CoInitialize(NULL);

	BuildObjects();

	return(true);
}
#define _WITH_DIRECT2D_IMAGE_EFFECT
#ifdef _WITH_DIRECT2D
void CGameFramework::CreateDirect2DDevice()
{
	UINT nD3D11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	nD3D11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ID3D11Device *pd3d11Device = NULL;
	HRESULT hResult = ::D3D11On12CreateDevice(m_pd3dDevice, nD3D11DeviceFlags, NULL, 0, (IUnknown **)&m_pd3dCommandQueue, 1, 0, &pd3d11Device, &m_pd3d11DeviceContext, NULL);
	hResult = pd3d11Device->QueryInterface(__uuidof(ID3D11On12Device), (void **)&m_pd3d11On12Device);
	if (pd3d11Device) pd3d11Device->Release();

	D2D1_FACTORY_OPTIONS nD2DFactoryOptions = { D2D1_DEBUG_LEVEL_NONE };
#if defined(_DEBUG)
	nD2DFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	hResult = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &nD2DFactoryOptions, (void **)&m_pd2dFactory);

	IDXGIDevice *pdxgiDevice = NULL;
	hResult = m_pd3d11On12Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pdxgiDevice);
	hResult = m_pd2dFactory->CreateDevice(pdxgiDevice, &m_pd2dDevice);
	hResult = m_pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2dDeviceContext);
	hResult = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&m_pdWriteFactory);

	if (pdxgiDevice) pdxgiDevice->Release();

	m_pd2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	m_pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0.3f, 0.0f, 0.0f, 0.5f), &m_pd2dbrBackground);
	m_pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)), &m_pd2dbrBorder);

	hResult = m_pdWriteFactory->CreateTextFormat(L"맑은 고딕체", NULL, DWRITE_FONT_WEIGHT_DEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15.0f, L"en-US", &m_pdwFont);
	hResult = m_pdwFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	hResult = m_pdwFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 1.0f), &m_pd2dbrText);
	hResult = m_pdWriteFactory->CreateTextLayout(L"텍스트 레이아웃", 8, m_pdwFont, 4096.0f, 4096.0f, &m_pdwTextLayout);

#ifdef _WITH_DIRECT2D_IMAGE_EFFECT
	CoInitialize(NULL);
	hResult = ::CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void **)&m_pwicImagingFactory);

	hResult = m_pd2dFactory->CreateDrawingStateBlock(&m_pd2dsbDrawingState);
	hResult = m_pd2dDeviceContext->CreateEffect(CLSID_D2D1BitmapSource, &m_pd2dfxBitmapSource);
	hResult = m_pd2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_pd2dfxGaussianBlur);

	IWICBitmapDecoder *pwicBitmapDecoder;
	hResult = m_pwicImagingFactory->CreateDecoderFromFilename(L"UI/InGameUI/chatting_bg.png", NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pwicBitmapDecoder);
	IWICBitmapFrameDecode *pwicFrameDecode;
	pwicBitmapDecoder->GetFrame(0, &pwicFrameDecode);
	m_pwicImagingFactory->CreateFormatConverter(&m_pwicFormatConverter);
	m_pwicFormatConverter->Initialize(pwicFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	m_pd2dfxBitmapSource->SetValue(D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE, m_pwicFormatConverter);
	m_pd2dfxGaussianBlur->SetInputEffect(0, m_pd2dfxBitmapSource);
	if (pwicBitmapDecoder) pwicBitmapDecoder->Release();
	if (pwicFrameDecode) pwicFrameDecode->Release();
#endif
}
#endif
void CGameFramework::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1 **)&m_pdxgiSwapChain);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE; //false는 전체모드. 이거 전체모드 할라면 
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue, &dxgiSwapChainDesc, (IDXGISwapChain **)&m_pdxgiSwapChain);
	//hResult = m_pdxgiSwapChain->SetFullscreenState(false, NULL);
	//if (hResult == E_FAIL)
	//	return;
	//m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	////SetFullScreenState함수를 호출해주었기 때문에 ResizeBuffers함수를 호출해줘야함.
	//m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth, m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
#endif

	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	hResult = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetViews();
#endif
}

void CGameFramework::CreateDirect3DDevice()
{
	HRESULT hResult;

	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug *pd3dDebugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void **)&pd3dDebugController);
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void **)&m_pdxgiFactory);

	IDXGIAdapter1 *pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice))) break;
	}

	if (!pd3dAdapter)
	{
		m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIFactory4), (void **)&pd3dAdapter);
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice);
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hResult = m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;

	hResult = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&m_pd3dFence);
	for (UINT i = 0; i < m_nSwapChainBuffers; i++) m_nFenceValues[i] = 0;

	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::gnCbvSrvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	if (pd3dAdapter) pd3dAdapter->Release();
}

void CGameFramework::CreateCommandQueueAndList()
{
	HRESULT hResult;

	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hResult = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void **)&m_pd3dCommandQueue);
	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&m_pd3dCommandAllocator);
	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&m_pd3dCommandList);
	hResult = m_pd3dCommandList->Close();
}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dRtvDescriptorHeap);
	m_nRtvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dDsvDescriptorHeap);
	m_nDsvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void CGameFramework::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_nSwapChainBuffers; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void **)&m_ppd3dSwapChainBackBuffers[i]);
		m_pd3dDevice->CreateRenderTargetView(m_ppd3dSwapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}

#ifdef _WITH_DIRECT2D
	float fxDPI, fyDPI;
	m_pd2dFactory->GetDesktopDpi(&fxDPI, &fyDPI); //UINT GetDpiForWindow(HWND hwnd);

	D2D1_BITMAP_PROPERTIES1 d2dBitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), fxDPI, fyDPI);

	for (UINT i = 0; i < m_nSwapChainBuffers; i++)
	{
		D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
		m_pd3d11On12Device->CreateWrappedResource(m_ppd3dSwapChainBackBuffers[i], &d3d11Flags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, __uuidof(ID3D11Resource), (void **)&m_ppd3d11WrappedBackBuffers[i]);

		IDXGISurface *pdxgiSurface = NULL;
		m_ppd3d11WrappedBackBuffers[i]->QueryInterface(__uuidof(IDXGISurface), (void **)&pdxgiSurface);
		m_pd2dDeviceContext->CreateBitmapFromDxgiSurface(pdxgiSurface, &d2dBitmapProperties, &m_ppd2dRenderTargets[i]);
		if (pdxgiSurface) pdxgiSurface->Release();
	}
#endif
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = m_nWndClientWidth;
	d3dResourceDesc.Height = m_nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask = 1;
	d3dHeapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void **)&m_pd3dDepthStencilBuffer);

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
}

void CGameFramework::ChangeSwapChainState()
{
	WaitForGpuComplete();

	BOOL bFullScreenState = FALSE;
	m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
	m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

	DXGI_MODE_DESC dxgiTargetParameters;
	dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiTargetParameters.Width = m_nWndClientWidth;
	dxgiTargetParameters.Height = m_nWndClientHeight;
	dxgiTargetParameters.RefreshRate.Numerator = 60;
	dxgiTargetParameters.RefreshRate.Denominator = 1;
	dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth, m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);

	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetViews();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_LBUTTONDOWN:
		/*	::SetCapture(hWnd);
			::GetCursorPos(&m_ptRightOldCursorPos);*/

		case WM_RBUTTONDOWN:
			::SetCapture(hWnd);
			::GetCursorPos(&m_ptOldCursorPos);
			break;
		case WM_RBUTTONUP:
			::ReleaseCapture();
		case WM_LBUTTONUP:
			break;
			break;
		case WM_MOUSEMOVE:
			break;
		default:
			break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_KEYUP:
			switch (wParam)
			{
				case VK_ESCAPE:
					::PostQuitMessage(0);
					break;
				case VK_RETURN:
					break;
				case VK_F1:
				case VK_F2:
				case VK_F3:
				case VK_F4:
					m_pCamera = PLAYER->GetPlayer()->ChangeCamera((DWORD)(wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
					break;
				case VK_F6: {
					if (m_match == false) {
						CNETWORK->MatchPkt();
						m_match = true;
					}
					break;
				}
				case VK_F9:
					ChangeSwapChainState();
					break;
				case '1':
				case '2':
					PLAYER->GetPlayer()->SetTrackAnimationSet(0, int(wParam) - '1');
					break;
				default:
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

void CGameFramework::OnDestroy()
{
    ReleaseObjects();

	::CloseHandle(m_hFenceEvent);
#ifdef _WITH_DIRECT2D
	if (m_pd2dbrBackground) m_pd2dbrBackground->Release();
	if (m_pd2dbrBorder) m_pd2dbrBorder->Release();
	if (m_pdwFont) m_pdwFont->Release();
	if (m_pdwTextLayout) m_pdwTextLayout->Release();
	if (m_pd2dbrText) m_pd2dbrText->Release();
#ifdef _WITH_DIRECT2D_IMAGE_EFFECT
	if (m_pd2dfxBitmapSource) m_pd2dfxBitmapSource->Release();
	if (m_pd2dfxGaussianBlur) m_pd2dfxGaussianBlur->Release();
	if (m_pd2dsbDrawingState) m_pd2dsbDrawingState->Release();
	if (m_pwicFormatConverter) m_pwicFormatConverter->Release();
	if (m_pwicImagingFactory) m_pwicImagingFactory->Release();
#endif

	if (m_pd2dDeviceContext) m_pd2dDeviceContext->Release();
	if (m_pd2dDevice) m_pd2dDevice->Release();
	if (m_pdWriteFactory) m_pdWriteFactory->Release();
	if (m_pd3d11On12Device) m_pd3d11On12Device->Release();
	if (m_pd3d11DeviceContext) m_pd3d11DeviceContext->Release();
	if (m_pd2dFactory) m_pd2dFactory->Release();

	for (int i = 0; i < m_nSwapChainBuffers; i++)
	{
		if (m_ppd3d11WrappedBackBuffers[i]) m_ppd3d11WrappedBackBuffers[i]->Release();
		if (m_ppd2dRenderTargets[i]) m_ppd2dRenderTargets[i]->Release();
	}
#endif


	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDsvDescriptorHeap) m_pd3dDsvDescriptorHeap->Release();

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();
	if (m_pd3dRtvDescriptorHeap) m_pd3dRtvDescriptorHeap->Release();

	if (m_pd3dCommandAllocator) m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandQueue) m_pd3dCommandQueue->Release();
	if (m_pd3dCommandList) m_pd3dCommandList->Release();

	if (m_pd3dFence) m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiFactory) m_pdxgiFactory->Release();

#if defined(_DEBUG)
	IDXGIDebug1	*pdxgiDebug = NULL;
	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void **)&pdxgiDebug);
	HRESULT hResult = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	pdxgiDebug->Release();
#endif
}

#define _WITH_TERRAIN_PLAYER

void CGameFramework::BuildObjects()
{
	CNETWORK->MakeServer(m_hWnd);
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	//SCENEMANAGER->
	m_pScene = new CScene();
	if (m_pScene)
	{
		m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
		//m_pScene->SetCollideBox();
	}
		//SCENEMANAGER->m_MapList[MENUSCENE] = new CMenuScene();
	//SCENEMANAGER->m_MapList[INGAME] = new CInGameScene();

	//for (auto&& p : SCENEMANAGER->m_MapList)
	//{
	//	//p.second->CreateShaderResourceViews(m_pd3dDevice,nullptr,)
	//	p.second->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
	//}


	//m_pScene = new CScene();
	//if (m_pScene) m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
	//SCENEMANAGER->m_MapList[MENUSCENE] = new CMenuScene();
	//SCENEMANAGER->m_MapList[INGAME] = new CInGameScene();

	//for (auto& p : SCENEMANAGER->m_MapList)
	//{
	//	//p.second->CreateShaderResourceViews(m_pd3dDevice,nullptr,)
	//	p.second->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
	//}

	//여기가 배치 구문이니 신경써야한다. 
#ifdef _WITH_TERRAIN_PLAYER
	PLAYER->Initialize(m_pd3dDevice, m_pd3dCommandList, m_pScene->GetGraphicsRootSignature(), m_pScene->m_pTerrain);

	PLAYER->GetPlayer()->SetPosition(XMFLOAT3(100, 0, 400));//XMFLOAT3(380.0f, SCENEMANAGER->m_MapList[INGAME]->m_pTerrain->GetHeight(380.0f, 680.0f), 680.0f));
	PLAYER->GetPlayer()->SetScale(XMFLOAT3(PLAYER->GetPlayer()-> m_BoundScale, PLAYER->GetPlayer()->m_BoundScale, PLAYER->GetPlayer()->m_BoundScale)); //박스도 151515배 여기여기0409
	PLAYER->GetPlayer()->SetOOBB(PLAYER->GetPlayer()->GetPosition(), XMFLOAT3(7,10,7), XMFLOAT4(0, 0, 0, 1));
	
	PLAYER->GetOtherPlayer()->SetPosition(XMFLOAT3(100, 0, 400));//XMFLOAT3(380.0f, SCENEMANAGER->m_MapList[INGAME]->m_pTerrain->GetHeight(380.0f, 680.0f), 680.0f));
	//PLAYER->GetOtherPlayer()->SetScale(XMFLOAT3(PLAYER->GetOtherPlayer()->m_,20, 40)); //박스도 151515배 여기여기0409
	PLAYER->GetOtherPlayer()->SetScale(XMFLOAT3(PLAYER->GetOtherPlayer()->m_BoundScale, PLAYER->GetOtherPlayer()->m_BoundScale, PLAYER->GetOtherPlayer()->m_BoundScale)); //박스도 151515배 여기여기0409

	PLAYER->GetOtherPlayer()->SetOOBB(PLAYER->GetOtherPlayer()->GetPosition(), XMFLOAT3(7, 10, 7), XMFLOAT4(0, 0, 0, 1));


	//if (m_pScene) m_pScene->MakeOtherPlayer(m_pd3dDevice, m_pd3dCommandList);
	//MAKE OTHER PLAYER
	
	//PLAYER->GetPlayer()->SetMesh()
	//PLAYER->GetPlayer()->FindAndSetSkinnedMesh()
	//if (PLAYER->GetPlayer()->GetRootSkinnedGameObject()->m_pMesh != nullptr)
		//int i = 0;
	//PLAYER->GetPlayer()->SetCollideBox();
																									 //PLAYER->MakeOtherPlayers(m_pd3dDevice, m_pd3dCommandList, SCENEMANAGER->m_MapList[INGAME]->GetGraphicsRootSignature(), SCENEMANAGER->m_MapList[INGAME]->m_pTerrain);
	
#else
	CAirplanePlayer *pPlayer = new CAirplanePlayer(m_pd3dDevice, m_pd3dCommandList, m_pScene->GetGraphicsRootSignature(), NULL);
	pPlayer->SetPosition(XMFLOAT3(425.0f, 240.0f, 640.0f));
#endif
	//m_pScene->BuildObjectsAfterPlayer(m_pd3dDevice, m_pd3dCommandList);
	//m_pScene->m_pPlayer = m_pPlayer;// = pPlayer;// = PLAYER->GetInstance()->GetPlayer();
	m_pCamera = PLAYER->GetPlayer()->GetCamera();
	
	//m_pCamera->SetMode(THIRD_PERSON_CAMERA);

	m_pd3dCommandList->Close();
	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	//if (SCENEMANAGER->m_MapList[INGAME]) SCENEMANAGER->m_MapList[INGAME]->ReleaseUploadBuffers();
	//if (m_pPlayer) PLAYER->GetPlayer()->ReleaseUploadBuffers();

	m_GameTimer.Reset();
}

void CGameFramework::ReleaseObjects()
{
	//if (m_pPlayer) PLAYER->GetPlayer()->Release();

	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	if (GetKeyboardState(pKeysBuffer) && m_pScene)
		bProcessedByScene = m_pScene->ProcessInput(pKeysBuffer);
	if (!bProcessedByScene&& PLAYER->GetPlayer()->GetAllowKey())
	{
		if (pKeysBuffer[VK_RETURN] & 0xF0)
		{
			SCENEMANAGER->SetScene(INGAME);

		}
		//PLAYER->GetPlayer()->GetDirectiond() = 0;
		dwDirection = 0;
		otherPlayerDirection = 0;
		if (pKeysBuffer[VK_UP] & 0xF0)
		{
			PLAYER->GetPlayer()->SetPlayerState(RUN);
			//PLAYER->GetOtherPlayer()->SetPlayerState(RUN);

			dwDirection |= DIR_FORWARD;

			//PLAYER->GetPlayer()->SetTrackAnimationSet(0, CPlayer::PlayerState::RUN);
		}
		if (pKeysBuffer[VK_DOWN] & 0xF0)
		{
			PLAYER->GetPlayer()->SetPlayerState(RUN);
			dwDirection |= DIR_BACKWARD;
		}
		if (pKeysBuffer[VK_LEFT] & 0xF0) 
		{
			PLAYER->GetPlayer()->SetPlayerState(RUN);
			dwDirection |= DIR_LEFT;
		} 
		if (pKeysBuffer[VK_RIGHT] & 0xF0) 
		{
			PLAYER->GetPlayer()->SetPlayerState(RUN);
			dwDirection |= DIR_RIGHT;
		}

		//2플레이어
		//if (pKeysBuffer[VK_HANGUL] & 0xF0)
		//{
		//	//PLAYER->GetPlayer()->SetPlayerState(RUN);
		//	PLAYER->GetOtherPlayer()->SetPlayerState(RUN);
		//	otherPlayerDirection |= DIR_FORWARD;

		//	//PLAYER->GetPlayer()->SetTrackAnimationSet(0, CPlayer::PlayerState::RUN);
		//}
		//if (pKeysBuffer[VK_HELP] & 0xF0)
		//{
		//	PLAYER->GetOtherPlayer()->SetPlayerState(RUN);
		//	otherPlayerDirection |= DIR_BACKWARD;
		//}
		//if (pKeysBuffer[VK_INSERT] & 0xF0)
		//{
		//	PLAYER->GetOtherPlayer()->SetPlayerState(RUN);
		//	otherPlayerDirection |= DIR_LEFT;
		//}
		//if (pKeysBuffer[VK_LCONTROL] & 0xF0)
		//{
		//	PLAYER->GetOtherPlayer()->SetPlayerState(RUN);
		//	otherPlayerDirection |= DIR_RIGHT;
		//}

		if (pKeysBuffer[VK_PRIOR] & 0xF0) {
			//PLAYER->GetPlayer()->SetPlayerState(RUN);
			dwDirection |= DIR_UP;
		} 
		if (pKeysBuffer[VK_NEXT] & 0xF0)
		{
			//PLAYER->GetPlayer()->SetPlayerState(RUN); 
			dwDirection |= DIR_DOWN;
		}
		if (pKeysBuffer[VK_SPACE] & 0xF0)
		{
			PLAYER->GetPlayer()->SetPlayerState(JUMP);
			PLAYER->GetOtherPlayer()->SetPlayerState(JUMP);

			//PLAYER->GetPlayer()->m_pAnimationController->SetTrackPosition(0, 0); //여기
		}
		if (pKeysBuffer[VK_LBUTTON] & 0xF0) //왜인지 모르겠으나 LButton하면 Rboutton누른걸로 설정
		{
			PLAYER->GetPlayer()->SetPlayerState(ATTACK);
			PLAYER->GetOtherPlayer()->SetPlayerState(ATTACK);

		}

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		if (GetCapture() == m_hWnd)
		{
			SetCursor(NULL);
			GetCursorPos(&ptCursorPos);
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 4.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 4.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		if ((otherPlayerDirection != 0)||(dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				//if (pKeysBuffer[VK_LBUTTON] & 0xF0) //왜인지 모르겠으나 LButton하면 Rboutton누른걸로 설정
				//	PLAYER->GetPlayer()->Rotate(cyDelta, 0.0f, -cxDelta);
				if (pKeysBuffer[VK_RBUTTON] & 0xF0) //왜인지 모르겠으나 LButton하면 Rboutton누른걸로 설
				{
					PLAYER->GetPlayer()->Rotate(cyDelta, cxDelta, 0.0f);
					PLAYER->GetOtherPlayer()->Rotate(cyDelta, cxDelta, 0.0f);

				}
			}
			if (dwDirection )
			{
				//PLAYER->GetPlayer()->Move(dwDirection, 12.25f, true);
				CNETWORK->StatePkt(dwDirection, m_GameTimer.GetTimeElapsed()); //서버에 키상태전송
			}
		}
	}
	//PLAYER->GetPlayer()->SetAllowKey(true);
	PLAYER->GetPlayer()->Update(m_GameTimer.GetTimeElapsed());
	PLAYER->GetOtherPlayer()->Update(m_GameTimer.GetTimeElapsed());

}

void CGameFramework::AnimateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	/*for (auto&& p : SCENEMANAGER->m_MapList)
	{
		p.second->AnimateObjects(fTimeElapsed);

	}*/
	m_pScene->AnimateObjects(fTimeElapsed);
	//SCENEMANAGER->m_MapList[SCENEMANAGER->GetSceneType()]->AnimateObjects(fTimeElapsed);


	PLAYER->GetPlayer()->Animate(fTimeElapsed);
	PLAYER->GetOtherPlayer()->Animate(fTimeElapsed);

	PLAYER->GetPlayer()->UpdateTransform(NULL);
	PLAYER->GetOtherPlayer()->UpdateTransform(NULL);


	
#ifdef _WITH_DIRECT2D_IMAGE_EFFECT
	static UINT64 i = 0;
	if (++i % 15) return;

	static float fColors[4] = { 0.0f, 0.478f, 0.8f, 1.0f };
	fColors[1] += 0.01f;
	if (fColors[1] > 1.0f) fColors[1] = 0.0f;
	m_pd2dfxGaussianBlur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 0.3f + fColors[1] * 10.0f);
#endif
}

void CGameFramework::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CGameFramework::MoveToNextFrame()
{
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	//m_nSwapChainBufferIndex = (m_nSwapChainBufferIndex + 1) % m_nSwapChainBuffers;

	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

//#define _WITH_PLAYER_TOP

void CGameFramework::FrameAdvance()
{    
	m_GameTimer.Tick(60.0f);
	
	ProcessInput();

    AnimateObjects();

	HRESULT hResult = m_pd3dCommandAllocator->Reset();
	hResult = m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex];
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; //다렉투디로 바꿀떄 비활성화
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	m_pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	m_pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);


	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_nSwapChainBufferIndex * m_nRtvDescriptorIncrementSize);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);
	if (m_pScene)
	{
		m_pScene->Render(m_pd3dCommandList, m_pCamera);
	
	}



#ifdef _WITH_PLAYER_TOP
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
#endif
	if (PLAYER->GetPlayer()!=NULL) PLAYER->GetPlayer()->Render(m_pd3dCommandList, m_pCamera);
	if (PLAYER->GetOtherPlayer() != NULL) PLAYER->GetOtherPlayer()->Render(m_pd3dCommandList, m_pCamera);
	cout << "X: " << PLAYER->GetPlayer()->GetPosition().x << "Y: " << PLAYER->GetPlayer()->GetPosition().y << "Z: " << PLAYER->GetPlayer()->GetPosition().z << endl;
	if (prePosition.x != PLAYER->GetPlayer()->GetPosition().x && prePosition.z != PLAYER->GetPlayer()->GetPosition().z) {
		CNETWORK->Pos(PLAYER->GetPlayer()->GetPosition());
		prePosition = PLAYER->GetPlayer()->GetPosition();
	}
	if (m_pScene)
	{
		m_pScene->CheckObjectByObjectCollisions();
	}
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	hResult = m_pd3dCommandList->Close();
	
	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();
#ifndef _WITH_DIRECT2D
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
#endif



#ifdef _WITH_DIRECT2D
	//Direct2D Drawing
	m_pd3d11On12Device->AcquireWrappedResources(&m_ppd3d11WrappedBackBuffers[m_nSwapChainBufferIndex], 1);
	m_pd2dDeviceContext->SetTarget(m_ppd2dRenderTargets[m_nSwapChainBufferIndex]);

	m_pd2dDeviceContext->BeginDraw();

	m_pd2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
#ifdef _WITH_DIRECT2D_IMAGE_EFFECT
	m_pd2dDeviceContext->DrawImage(m_pd2dfxBitmapSource);
	//	m_pd2dDeviceContext->DrawRectangle(rcText, m_pd2dbrBackground);

	//	m_pd2dDeviceContext->DrawRectangle(&rcText, m_pd2dbrBorder);
	//	m_pd2dDeviceContext->FillRectangle(&rcText, m_pd2dbrBackground);
#endif
	D2D1_SIZE_F szRenderTarget = m_ppd2dRenderTargets[m_nSwapChainBufferIndex]->GetSize(); //D2D1::SizeF(0, szRenderTarget.height*0.2);/
	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	D2D1_RECT_F rcUpperText = D2D1::RectF(0, 0, szRenderTarget.width * 0.5, szRenderTarget.height*0.2);
	m_pd2dDeviceContext->DrawTextW(m_pszFrameRate, (UINT32)wcslen(m_pszFrameRate), m_pdwFont, &rcUpperText, m_pd2dbrText);

	D2D1_RECT_F rcLowerText = D2D1::RectF(0, 0 , szRenderTarget.width * 0.5, szRenderTarget.height);
	m_pd2dDeviceContext->DrawTextW(L"트위치 채팅창 예시", (UINT32)wcslen(L"트위치 채팅창 예시"), m_pdwFont, &rcLowerText, m_pd2dbrText);


	m_pd2dDeviceContext->EndDraw();

	m_pd3d11On12Device->ReleaseWrappedResources(&m_ppd3d11WrappedBackBuffers[m_nSwapChainBufferIndex], 1);

	m_pd3d11DeviceContext->Flush();
#endif
#ifdef _WITH_PRESENT_PARAMETERS
	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.R = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
#else
#ifdef _WITH_SYNCH_SWAPCHAIN
	m_pdxgiSwapChain->Present(1, 0);
#else
	m_pdxgiSwapChain->Present(0, 0);
#endif
#endif

//	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	MoveToNextFrame();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	size_t nLength = _tcslen(m_pszFrameRate);
	XMFLOAT3 xmf3Position = PLAYER->GetPlayer()->GetPosition();
	_stprintf_s(m_pszFrameRate + nLength, 70 - nLength, _T("(%4f, %4f, %4f)"), xmf3Position.x, xmf3Position.y, xmf3Position.z);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}

