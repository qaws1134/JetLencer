#include "stdafx.h"
#include "Mouse.h"
#include "Gui.h"

CMouse::CMouse() 
	: m_fSizeCheckTime(0.f)
	,m_fSizeCheckSpeed(0.05f)
	,m_eMouse(MOUSE::IDLE)
	,m_ePreMouse(MOUSE::END)
	,m_fHitSpeed(0.1f)
	,m_fHitTime(0.f)
{
}
CMouse::~CMouse()
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

HRESULT CMouse::Ready_GameObject()
{

	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = D3DXVECTOR3(1.f, 1.f, 0.f);
	ShowCursor(false);
	m_tFrame.wstrObjKey = L"Mouse_Idle";
	m_tFrame.fMaxFrame = 0;
	m_eRenderId = RENDERID::UI;

	pHit = CGui::Create(UI::MOUSE_HIT);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, pHit);
	return S_OK;
}

int CMouse::Update_GameObject()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	m_vMouse = {float(pt.x),float(pt.y),0.f };
	m_tInfo.vPos = m_vMouse;
	ShowCursor(false);

	SizeCheck();

	if (IdleTime())
	{
		m_eMouse = MOUSE::IDLE;
	}
	State_Change();
	
	Set_Texture();
	return OBJ_NOEVENT;
}

void CMouse::Late_Update_GameObject()
{
}
void CMouse::WriteMatrix()
{
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x , m_tInfo.vPos.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;

	if (!m_bCenter)
	{
		float fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
		m_fCenterX = fCenterX;
		m_fCenterY = fCenterY;
	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));

}

bool CMouse::IdleTime()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fHitTime += fTime;
	if (m_fHitTime > m_fHitSpeed)
	{
		return true;
	}
	return false;
}

void CMouse::SizeCheck()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fSizeCheckTime += fTime;
	if (m_fSizeCheckTime > m_fSizeCheckSpeed)
	{
		m_fSizeCheckTime = 0.f;
		m_vPrePos = m_tInfo.vPos;
	}

	float fLength = D3DXVec3Length(&(m_vPrePos-m_tInfo.vPos));
	_vec3 vIncSize = m_tInfo.vSize*(fLength*0.001f);
	m_tInfo.vSize += vIncSize;
	
	if (m_tInfo.vSize.x > 1.25f)
		m_tInfo.vSize = { 1.25f,1.25f,0.f };

	else if (m_tInfo.vSize.x > 1.f)
		m_tInfo.vSize -= _vec3{ 0.01f,0.01f,0.f };

}

void CMouse::Render_GameObject()
{
	WriteMatrix();
}

void CMouse::Release_GameObject()
{
}

_vec3 CMouse::Get_MousePos()
{
	m_tInfo.vPos -= CScroll_Manager::Get_Scroll();
	return m_tInfo.vPos;
}

void CMouse::State_Change()
{
	switch (m_eMouse)
	{
	case MOUSE::IDLE:
		static_cast<CGui*>(pHit)->Set_Render(false);
		break;
	case MOUSE::HIT:
		static_cast<CGui*>(pHit)->Set_Render(true);
		pHit->Set_Pos(m_tInfo.vPos);
		break;
	}
	
}

