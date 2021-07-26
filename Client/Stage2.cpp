#include "stdafx.h"
#include "Stage2.h"
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
CStage2::CStage2() 
	: m_bEnd(false)
	, m_bStart(false)
	, m_fEndTime(0.f)
	, m_fEndSpeed(0.f)
{

}


CStage2::~CStage2()
{

}

CScene * CStage2::Create()
{
	CScene* pInstance = new CStage2;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CStage2::Ready_Scene()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage1.mp3");
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::SCENE_STAGE1);


	CGameObject* pObject = nullptr;

	pObject = CPlayer::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::PLAYER, pObject);

	pObject = CMouse::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::MOUSE, pObject);



	//�� ����Ʈ �ʱ�ȭ
	for (int i = 0; i < 5; i++)
	{
		m_listEnemy[0].emplace_back(L"Jet_Normal");
		m_listEnemy[0].emplace_back(L"Jet_Normal");
		m_listEnemy[1].emplace_back(L"Jet_Rocket");
		m_listEnemy[1].emplace_back(L"Jet_Normal");
		m_listEnemy[2].emplace_back(L"Jet_BulletSplash");
		m_listEnemy[2].emplace_back(L"Jet_Normal");  
	}

	m_pWaveText = CViewText::Create(L"Wave : %d / 3");
	m_pWaveText->Set_Pos(_vec3{ WINCX - 200.f,100.f,0.f });
	static_cast<CViewText*>(m_pWaveText)->Set_iPoint(1);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pWaveText);
	m_fSpawnTime = -1.f;  
	m_fSpawnSpeed = 1.f;

	m_fWaveTime = 0.f;
	m_fWaveSpeed= 1.f;
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

void CStage2::Update_Scene()
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
		//���̺� �ð� ����
		//���̺�ð��� �ӵ����� ũ�� ����Ÿ�� ����
		//����Ÿ�Ӹ��� ����ī��Ʈ�� �ش�
		//����
		if(!m_bEnd)
		{
			m_fSpawnTime += fTime;
			if (m_fSpawnTime > m_fSpawnSpeed)
			{
				//5���� �����Ǹ� ���̺� Ÿ�� �ʱ�ȭ
				if (m_iSpawnCount > 7)
				{
					if (CGameObject_Manager::Get_Instance()->Object_NullCheck(OBJID::ENEMY))
					{
						m_iWaveCount++;
						m_iSpawnCount = 0;

						if (m_iWaveCount > 1)
						{
							m_bEnd = true;
						}
						else
							static_cast<CViewText*>(m_pWaveText)->Set_iPoint((m_iWaveCount + 1));
					}
				}
				else
				{
					int irandIndx = rand() % 3;
					if (!m_listEnemy[irandIndx].empty())
					{
						//��ġ�� �ʱ�ȭ
						m_vecSpawnPos.swap(vector<_vec3>());

						//��ġ ���� 
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
	}

	if (m_bEnd)
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
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_P))
	{
		m_bEnd = true;
	}


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CSoundMgr::Get_Instance()->StopAll();
		//CSoundMgr::Get_Instance()->PlaySound(L"Stage_Victory1.wav", CSoundMgr::STAGE_VICTORY);
		CSpawn_Manager::Destroy_Instance();
		CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
	}

	CSpawn_Manager::Get_Instance()->Update_MultiSpawn();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
	CScroll_Manager::Scroll_Lock();
}

void CStage2::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage2::Release_Scene()
{

	CSpawn_Manager::Destroy_Instance();
	CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE_SEL);
}
