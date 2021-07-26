#include "stdafx.h"
#include "StageBoss.h"
#include "Terrain.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "Mouse.h"
#include "Spawn_Manager.h"
#include "Scroll_Manager.h"
#include "ViewText.h"
#include "Arrow_Offscreen.h"
#include "Serpent.h"
#include "SoundMgr.h"
CStageBoss::CStageBoss() :m_bStart(false),m_bSoundStart(false), m_bBoss(false)
{

}


CStageBoss::~CStageBoss()
{

}

CScene * CStageBoss::Create()
{
	CScene* pInstance = new CStageBoss;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CStageBoss::Ready_Scene()
{


	//CSoundMgr::Get_Instance()->PlayBGM(L"StageBoss.mp3");
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::SCENE_STAGE_EMPTY);


	CGameObject* pObject = nullptr;



	pObject = CPlayer::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::PLAYER, pObject);

	pObject = CMouse::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::MOUSE, pObject);

	//m_pBoss = CSerpent::Create(_vec3{ float(Map_Width >> 1) -400.f,float(Map_Height) ,0.f });
	
	m_fStartSpeed = 2.f;
	m_pStart_End = CGui::Create(UI::STAGE_START_END);
	static_cast<CGui*>(m_pStart_End)->Set_Start(true);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pStart_End);

	pObject = CGui::Create(UI::START_COUNT3);
	m_vecStageStart.emplace_back(pObject);
	pObject = CGui::Create(UI::START_COUNT2);
	m_vecStageStart.emplace_back(pObject);
	pObject = CGui::Create(UI::START_COUNT1);
	m_vecStageStart.emplace_back(pObject);
	pObject = CGui::Create(UI::STAGE_ENGAGE);
	m_vecStageStart.emplace_back(pObject);
	pObject = CGui::Create(UI::STAGE_VICTORY);
	m_vecStageStart.emplace_back(pObject);

	for (auto& iter : m_vecStageStart)
	{
		iter->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1) - 200.f,0.f });
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}

	m_fEndSpeed = 3.f;
	

	return S_OK;
}

void CStageBoss::Update_Scene()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bStart)
	{

		m_fStartTime += fTime;
		if (m_fStartTime > m_fStartSpeed)
		{
			if (m_iStageUiIndex < 4)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Stage_CountDown.wav", CSoundMgr::STAGE_COUNT);
				static_cast<CGui*>(m_vecStageStart[m_iStageUiIndex])->Set_Action(true);
				if (static_cast<CGui*>(m_vecStageStart[m_iStageUiIndex])->Get_Start())
				{
					m_iStageUiIndex++;
				}
			}
			else
			{
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Start(true);
				m_bStart = true;
			}
		}
	}
	else if (m_bStart&&!m_bBoss)
	{
		m_pBoss = CSerpent::Create(_vec3{ float(Map_Width >> 1) -400.f,float(Map_Height) ,0.f });
		m_bBoss = true;
	}

	if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_AllTrueMod())
	{
		CSoundMgr::Get_Instance()->StopAll();
		m_bSoundStart = false;
	}
	else
	{
		if (!m_bSoundStart)
		{
			CSoundMgr::Get_Instance()->PlayBGM(L"StageBoss.mp3");
			m_bSoundStart = true;
		}
	}
	if (CGameObject_Manager::Get_Instance()->Get_GameEnd())
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		static_cast<CGui*>(m_pStart_End)->Set_Action(true);
		if (static_cast<CGui*>(m_pStart_End)->Get_Start())
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Stage_Victory1.wav", CSoundMgr::STAGE_VICTORY);
			static_cast<CGui*>(m_vecStageStart.back())->Set_Action(true);
		}
		m_fEndTime += fTime;
		if (m_fEndTime > m_fEndSpeed)
		{
			CSoundMgr::Get_Instance()->StopAll();
			Release_Scene();
		}
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	}
	CSpawn_Manager::Get_Instance()->Update_MultiSpawn();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();

}

void CStageBoss::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStageBoss::Release_Scene()
{
	CSpawn_Manager::Destroy_Instance();
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
}
