#include "stdafx.h"
#include "Burner.h"
#include "Prefab_Manager.h"

CBurner::CBurner()
	:m_ePreBurnerState(BURNER::END)
	, m_eBurnerState(BURNER::END)
	, m_bRender(true)
	, m_bLoop(false)

{
}


CBurner::~CBurner()
{
}

CGameObject * CBurner::Create()
{
	CGameObject* pInstance = new CBurner;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBurner::Ready_GameObject()
{
	m_tFrame.wstrObjKey = L"Player";
	m_tFrame.wstrStateKey = L"brn";
	m_eRenderId = RENDERID::BURNER;
	m_tInfo.vSize = { 1.3f,1.3f,0.f };

	m_bCenter = true;

	return S_OK;

}

int CBurner::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	State_Change();
	Frame_Change();

	return OBJ_NOEVENT;
}

void CBurner::Late_Update_GameObject()
{
	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CBurner::Render_GameObject()
{
	if (!m_pTexInfo)
		return;
	if (m_bRender)
	{
		m_fCenterX = float(m_pTexInfo->tImageInfo.Width);
		m_fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
		WriteMatrix();
	}
}

void CBurner::Release_GameObject()
{
}

void CBurner::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bLoop)
		{
			m_tFrame.fStartFrame = 0;
		}
		else
 			m_eBurnerState = BURNER::AFTER;
	}
}
void CBurner::State_Change()
{
	if (m_ePreBurnerState != m_eBurnerState)
	{
		switch (m_eBurnerState)
		{
		case BURNER::IDLE:
			m_eBurnerState = BURNER::IDLE;
			m_bRender = false;
			break;
		case BURNER::START:
			m_tFrame.wstrStateKey = L"brn_start";
			m_bLoop = true;
			break;
		case BURNER::START_OFF:
			m_tFrame.wstrStateKey= L"brn_off";
			m_bLoop = true;
			break;
		case BURNER::ACCEL:
			m_tFrame.wstrStateKey = L"brn";
			m_tFrame.fFrameSpeed = 20.f;
			m_bLoop = true;
			break;
		case BURNER::AFTER:
			m_tFrame.wstrStateKey = L"Aftbrn";
			m_tFrame.fFrameSpeed = 10.f;
			m_bLoop = true;
			break;
		case BURNER::MEGA:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.fFrameSpeed = 10.f;
			m_tFrame.wstrStateKey = L"Aftbrn_mega";
			m_bLoop = false;
			CScroll_Manager::Shake(12.f, 0.5f);
			break;
		case BURNER::BURST:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.wstrStateKey= L"Aftbrn_burst";
			m_tFrame.fFrameSpeed = 10.f;
			m_bLoop = false;
			CScroll_Manager::Shake(5.f, 0.4f);
			break;
		}
		m_ePreBurnerState = m_eBurnerState;
		m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Player" + m_tFrame.wstrStateKey)->iMax_Index;
		m_tFrame.fStartFrame = 0;
		if (m_eBurnerState != BURNER::IDLE)
			m_bRender = true;
	}
}
