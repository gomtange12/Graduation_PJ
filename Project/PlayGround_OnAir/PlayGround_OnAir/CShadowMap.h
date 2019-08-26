#pragma once
class CShadowMap
{
	D3D12_VIEWPORT m_ViewPort;
	D3D12_RECT	m_ScissorRect;

	UINT m_Width = 0;
	UINT m_Height = 0;
	DXGI_FORMAT m_Format = DXGI_FORMAT_R24G8_TYPELESS;
	ID3D12Resource* m_ShadowMap = nullptr;
public:

	CShadowMap();
	~CShadowMap();
};

