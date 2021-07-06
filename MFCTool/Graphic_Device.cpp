#include "stdafx.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)
CGraphic_Device::CGraphic_Device()
	:m_pDevice(nullptr)
	,m_pSDK(nullptr)
	,m_pSprite(nullptr)
	,m_pFont(nullptr)
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device(); 
}

HRESULT CGraphic_Device::Ready_Graphic_Device()
{
	// 장치를 초기화 하는 일련의 과정 
	//1. 장치의 수준을 조사할 컴객체라는 놈을 생성해야됨. 
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 
	D3DCAPS9 d3dcaps; 
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9)); 

	//2. 장치의 수준을 조사하고 
	//D3DADAPTER_DEFAULT, 진단도구에 나열된 그래픽 카드의 정보를 조사하겠다.
	//HAL - Hardware Abstract Layer 
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		goto ERR; 

	DWORD vp= 0; // 버텍스 프로세싱  = 정점변환 + 조명연산
	//HW//TRANSFORM//AND//LIGHT//
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; //컴퓨터가. 세종대왕, 진돗개, 삼보. 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS)); 
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;// 0~ 255 
	d3dpp.BackBufferCount = 1; //여기 카운트는 디폴트 1 + 내가 넣어준 카운트 만큼 . 그래서 
	// 1을 넣어주면 총 2개 쓰겠다. 백버퍼를 ㅇㅋ  ?? 

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	//D3DSWAPEFFECT_DISCARD - 스왑체인 방식을 사용하겠다 라는 뜻이 된다. 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = TRUE; //TRUE 일 경우 창모드 , FALSE 일경우 전체화면 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 풀스크린일때 어떻게 처리할 것인지에 대한 처리드.. 이건 컴퓨터가 알아서 해줄 문제. 
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//3. 그 수준을 토대로 장치를 제어하는 컴객체를 생성할 것이다. 
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
		goto ERR; 

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		goto ERR; 

	D3DXFONT_DESCW tFontDesc ;
	ZeroMemory(&tFontDesc, sizeof(D3DXFONT_DESCW)); 
	
	tFontDesc.Height = 20; 
	tFontDesc.Width = 10;
	tFontDesc.Weight = FW_HEAVY; 
	tFontDesc.CharSet = HANGEUL_CHARSET; 
	lstrcpy(tFontDesc.FaceName, L"CookieRun Black");
	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &tFontDesc, &m_pFont)))
		goto ERR; 

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		goto ERR;

	return S_OK;
ERR:
	AfxMessageBox(L"Ready_Graphic_Device Failed");
	return E_FAIL;
}

void CGraphic_Device::Release_Graphic_Device()
{
	if (m_pLine)
		m_pLine->Release(); 

	if (m_pFont)
		m_pFont->Release(); 

	if (m_pSprite)
		m_pSprite->Release(); 

	if (m_pDevice)
		m_pDevice->Release(); 

	if (m_pSDK)
		m_pSDK->Release(); 
}

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 0,0,255), 0.f, 0);
	m_pDevice->BeginScene(); 
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWnd /*= nullptr*/)
{
	m_pSprite->End(); 
	m_pDevice->EndScene(); 
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr); 
}
