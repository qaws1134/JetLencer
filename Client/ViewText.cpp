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

CViewText::CViewText() : m_wstrText(L"")
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

	return S_OK;
}



void CViewText::State_Change()
{
	TCHAR sztext[50] = L"";
	swprintf_s(sztext, m_wstrText.c_str(), m_fPoint);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), sztext, lstrlen(sztext), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

}

int CViewText::Update_GameObject()
{

	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}
