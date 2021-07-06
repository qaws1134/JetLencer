#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

HRESULT CMouse::Ready_GameObject()
{
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = D3DXVECTOR3(1.f, 1.f, 0.f);
	ShowCursor(false);
	m_tFrame.wstrObjKey = L"Mouse_Idle";
	m_tFrame.fMaxFrame = 0;
	m_eRenderId = RENDERID::UI;
	return S_OK;
}

int CMouse::Update_GameObject()
{
	_vec3 vScroll= CScroll_Manager::Get_Scroll();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	m_vMouse = {float(pt.x),float(pt.y),0.f };

	m_vMouse -= vScroll;

	m_tInfo.vPos = m_vMouse;
	return OBJ_NOEVENT;
}

void CMouse::Late_Update_GameObject()
{
	Set_Texture();
}

void CMouse::Render_GameObject()
{
	WriteMatrix();
}

void CMouse::Release_GameObject()
{
}

CGameObject * CMouse::Create()
{
	CGameObject* pInstance = new CMouse;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CMouse::State_Change()
{
}

