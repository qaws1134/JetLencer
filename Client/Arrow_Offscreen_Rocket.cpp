#include "stdafx.h"
#include "Arrow_Offscreen_Rocket.h"
#include "Prefab_Manager.h"

CArrow_Offscreen_Rocket::CArrow_Offscreen_Rocket()
{
}


CArrow_Offscreen_Rocket::~CArrow_Offscreen_Rocket()
{
}

CGameObject * CArrow_Offscreen_Rocket::Create()
{
	CGameObject* pInstance = new CArrow_Offscreen_Rocket;
	const ANIMATION* pAniInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiMarker_rocket");
	pInstance->Set_Animation(pAniInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CArrow_Offscreen_Rocket::Ready_GameObject()
{
	m_eRenderId = RENDERID::UI;
	m_tInfo.vSize = { 1.2f,1.2f,0.f };
	m_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	m_tFrame.fMaxFrame = 4;
	m_tFrame.fStartFrame = 0;
	m_fColorTime = 0.f;
	m_fColorSpeed= 0.3f;

	m_bRender = false;
	return S_OK;
}

int CArrow_Offscreen_Rocket::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	State_Change();
	return OBJ_NOEVENT;
}

void CArrow_Offscreen_Rocket::State_Change()
{
	ArrowDistance_Change();
	ArrowPosition_Change();
	ArrowColor_Change();
}


void CArrow_Offscreen_Rocket::ArrowDistance_Change()
{
	if (m_ePreArrowState != m_eArrowState)
	{
		switch (m_eArrowState)
		{
		case ARROW::ROCKET_MARKER:
			m_bRender = true;
			break;
		case ARROW::END:
			m_bRender = false;
			break;
		}
		m_ePreArrowState = m_eArrowState;
	}
}
void CArrow_Offscreen_Rocket::ArrowPosition_Change()
{

	if (m_tInfo.vPos.x > WINCX)
		m_tInfo.vPos.x = WINCX - 15;
	if (m_tInfo.vPos.y > WINCY)
		m_tInfo.vPos.y = WINCY - 15;

	if (m_tInfo.vPos.x < 0)
		m_tInfo.vPos.x = 15.f;
	if (m_tInfo.vPos.y < 0)
		m_tInfo.vPos.y = 15.f;

}

void CArrow_Offscreen_Rocket::ArrowColor_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fColorTime += fTime;

	if (m_fColorTime > m_fColorSpeed)
	{
		m_fColorTime = 0.f;
		m_bColor = !m_bColor;
	}

	if (m_bColor)
	{
		m_tColor = { 255,255,0,0 };
	}
	else
	{
		m_tColor = { 255,255,255,255 };
	}



}
