#include "stdafx.h"
#include "Stage3.h"
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
CStage3::CStage3() 
	: m_bEnd(false)
	, m_bStart(false)
	, m_fEndTime(0.f)
	, m_fEndSpeed(0.f)
{

}


CStage3::~CStage3()
{

}

CScene * CStage3::Create()
{
	CScene* pInstance = new CStage3;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CStage3::Ready_Scene()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage2.mp3");
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::SCENE_STAGE2);
	CGameObject* pObject = nullptr;
	pObject = CPlayer::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::PLAYER, pObject);
	pObject = CMouse::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::MOUSE, pObject);

	//적 리스트 초기화
	for (int i = 0; i < 10; i++)
	{
		m_listEnemy[0].emplace_back(L"Jet_Normal");
		m_listEnemy[0].emplace_back(L"Jet_Rocket");
		m_listEnemy[1].emplace_back(L"Jet_Normal");
		m_listEnemy[1].emplace_back(L"Jet_Normal");
		m_listEnemy[2].emplace_back(L"Jet_BulletSplash");
		m_listEnemy[2].emplace_back(L"Jet_Sniper");
	}

	m_pWaveText = CViewText::Create(L"남은 시간 : %d초 / 60초");
	m_pWaveText->Set_Pos(_vec3{ WINCX - 220.f,100.f,0.f });
	static_cast<CViewText*>(m_pWaveText)->Set_iPoint(0);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pWaveText);
	m_fSpawnTime = -1.f;
	m_fSpawnSpeed = 1.f;

	m_fWaveTime = 0.f;
	m_fWaveSpeed = 1.f;
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

void CStage3::Update_Scene()
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
	else if (m_bStart)
	{
		//웨이브 시간 증가
		//웨이브시간이 속도보다 크면 스폰타임 시작
		//스폰타임마다 스폰카운트를 준다
		//스폰
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_fSecond += fTime;
		static_cast<CViewText*>(m_pWaveText)->Set_iPoint(int(m_fSecond));
		if (int(m_fSecond) >= 60)
		{
			m_bEnd = true;
		}
		m_fWaveTime += fTime;
		if (m_fWaveTime > m_fWaveSpeed)
		{
			m_fSpawnTime += fTime;
			if (m_fSpawnTime > m_fSpawnSpeed)
			{
				//5개가 스폰되면 웨이브 타임 초기화
				if (m_iSpawnCount > 5)
				{
					m_fWaveTime = 0.f;
					m_fWaveSpeed = 15.f;
					m_iSpawnCount = 0;
				}
				int irandIndx = rand() % 3;
				if (!m_listEnemy[irandIndx].empty())
				{
					//위치값 초기화
					m_vecSpawnPos.swap(vector<_vec3>());
					m_vecSpawnPos.clear();
					//위치 선정 
					m_vecSpawnPos.emplace_back(_vec3{ 200.f + float(Map_Width) ,float(rand() % 200) + float(Map_Height >> 1),0.f });
					m_vecSpawnPos.emplace_back(_vec3{ -200.f,float(rand() % 200) + float(Map_Height >> 1),0.f });
					m_vecSpawnPos.emplace_back(_vec3{ float(Map_Width >> 1) + float(rand() % 200),-100.f,0.f });

					m_fSpawnTime = 0.f;
					CSpawn_Manager::Spawn(m_listEnemy[irandIndx].front(), m_vecSpawnPos[irandIndx]);
					m_listEnemy[irandIndx].pop_front();
					m_iSpawnCount++;
				}
			}
		}
	}

	if (m_bEnd)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		static_cast<CGui*>(m_pStart_End)->Set_Action(true);
		if (static_cast<CGui*>(m_pStart_End)->Get_Start())
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Stage_Victory1.wav",CSoundMgr::STAGE_VICTORY);
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
		CSoundMgr::Get_Instance()->StopAll();
		CSpawn_Manager::Destroy_Instance();
		CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
	}

	CSpawn_Manager::Get_Instance()->Update_MultiSpawn();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();
}

void CStage3::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage3::Release_Scene()
{
	CSpawn_Manager::Destroy_Instance();
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE_SEL);

}
