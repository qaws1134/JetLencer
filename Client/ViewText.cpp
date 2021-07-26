#include "stdafx.h"
#include "ViewText.h"
#include "Prefab_Manager.h"


CGameObject * CViewText::Create(_vec3 vPos, wstring _wstrText)
{
	CGameObject* pInstance = new CViewText;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	pInstance->Set_Pos(vPos);
	static_cast<CViewText*>(pInstance)->Set_Text(_wstrText);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CGameObject * CViewText::Create(wstring _wstrText, TEXT::SIZE _eSize)
{
	CGameObject* pInstance = new CViewText;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	static_cast<CViewText*>(pInstance)->Set_Text(_wstrText);
	static_cast<CViewText*>(pInstance)->Set_TextSize(_eSize);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CGameObject * CViewText::Create(wstring _wstrText)
{
	CGameObject* pInstance = new CViewText;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	static_cast<CViewText*>(pInstance)->Set_Text(_wstrText);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}


CViewText::CViewText() : m_wstrText(L""), m_eTextSize(TEXT::REGULER)
{
}


CViewText::~CViewText()
{
}


HRESULT CViewText::Ready_GameObject()
{
	m_eRenderId = RENDERID::UI;
	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;
	m_bRender = true;
	return S_OK;
}



void CViewText::State_Change()
{

}

int CViewText::Update_GameObject()
{

	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CViewText::WriteMatrix()
{
	if (!m_bRender)
		return;

	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;

	if (!m_bCenter)
	{
		float fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
		m_fCenterX = fCenterX;
		m_fCenterY = fCenterY;
	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(
		m_pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));



	TCHAR sztext[200] = L"";
	swprintf_s(sztext, m_wstrText.c_str(),m_iPoint);
	switch (m_eTextSize)
	{
	case TEXT::SMALL:
		CGraphic_Device::Get_Instance()->Get_SmallFont()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), sztext, lstrlen(sztext), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case TEXT::REGULER:
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), sztext, lstrlen(sztext), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case TEXT::BIG:
		CGraphic_Device::Get_Instance()->Get_BigFont()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), sztext, lstrlen(sztext), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}


}
