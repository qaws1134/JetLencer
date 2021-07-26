#include "stdafx.h"
#include "Burner.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"
#include "SoundMgr.h"

CBurner::CBurner()
	:m_ePreBurnerState(BURNER::END)
	, m_eBurnerState(BURNER::END)
	, m_bRender(true)
	, m_bLoop(false)
	, m_fOffsetX(0.f)
	, m_bAniEnd(false)

{
}


CBurner::~CBurner()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_AFTERBURN);
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
	if (m_eBurnerState == BURNER::START)
	{
		CScroll_Manager::Shake(0.8f, 0.1f);
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_fSpawnTime += fTime;
		if (m_fSpawnTime > 0.1f)
		{
			m_fSpawnTime = 0.f;
			for (int i = 0; i < 3; i++)
			{
				CSpawn_Manager::Spawn(EFFECT::AFTERBURN_EFF, _vec3{ m_tInfo.vPos.x,m_tInfo.vPos.y - (float(rand() % 2 + 1)*10.f) ,0.f });
				CSpawn_Manager::Spawn(EFFECT::AFTERBURN_EFF, _vec3{ m_tInfo.vPos.x,m_tInfo.vPos.y + (float(rand() % 2 + 1)*10.f) ,0.f });
			}
		}
	}
	else if (m_eBurnerState == BURNER::START_OFF)
	{
		if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame-1)
		{
			m_bAniEnd = true;
			m_fOffsetX = 0.f;
		}
	}
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
		m_fCenterX = float(m_pTexInfo->tImageInfo.Width)+m_fOffsetX;
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
			m_bCenter = true;
			m_tFrame.fFrameSpeed = 20.f;
			m_bLoop = true;
			m_fOffsetX = 20.f;
			CSoundMgr::Get_Instance()->PlaySound(L"Player_AfterBurn.mp3", CSoundMgr::PLAYER_AFTERBURN);
			break;
		case BURNER::START_OFF:
			m_tFrame.wstrStateKey= L"brn_start_off";
			m_tFrame.fFrameSpeed = 20.f;
			m_fOffsetX = 20.f;
			m_bCenter = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_AFTERBURN);
			break;
		case BURNER::ACCEL:
			m_tFrame.wstrStateKey = L"brn";
			m_tFrame.fFrameSpeed = 20.f;
			m_bLoop = true;
			m_bCenter = true;
			break;
		case BURNER::AFTER:
			m_tFrame.wstrStateKey = L"Aftbrn";
			m_tFrame.fFrameSpeed = 10.f;

			m_bCenter = true;
			m_bLoop = true;
			break;
		case BURNER::MEGA:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.fFrameSpeed = 10.f;
			m_tFrame.wstrStateKey = L"Aftbrn_mega";
			m_bLoop = false;

			m_bCenter = true;
			CScroll_Manager::Shake(12.f, 0.5f);
			break;
		case BURNER::BURST:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.wstrStateKey= L"Aftbrn_burst";
			m_tFrame.fFrameSpeed = 10.f;
			m_bLoop = false;
			CScroll_Manager::Shake(5.f, 0.4f);
			m_bCenter = true;
			break;
		}
		m_ePreBurnerState = m_eBurnerState;
		m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Player" + m_tFrame.wstrStateKey)->iMax_Index;
		m_tFrame.fStartFrame = 0;
		if (m_eBurnerState != BURNER::IDLE)
			m_bRender = true;
	}
}
