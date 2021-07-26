#include "stdafx.h"
#include "Scene_Manager.h"
#include "Stage.h"
#include "MyMenu.h"
#include "LoadOut.h"
#include "SelectStage.h"
#include "Stage2.h"
#include "Stage3.h"
#include "StageBoss.h"
IMPLEMENT_SINGLETON(CScene_Manager)
CScene_Manager::CScene_Manager()
	:m_eCurScene(END)
	, m_eNextScene(END)
	, m_pScene(nullptr)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_Scene_Manager(); 
}

HRESULT CScene_Manager::Change_Scene_Manager(const ID eID)
{
	m_eNextScene = eID; 
	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene); 
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_MENU:
			m_pScene = CMyMenu::Create();
			break;
		case CScene_Manager::SCENE_LOADOUT:
			m_pScene = CLoadOut::Create();
			break;
		case CScene_Manager::SCENE_STAGE_SEL:
			m_pScene = CSelectStage::Create();
			break;
		case CScene_Manager::SCENE_STAGE_EMPTY:
			m_pScene = CStage::Create();
			break;
		case CScene_Manager::SCENE_STAGE1:
			m_pScene = CStage2::Create(); 
			break;
		case CScene_Manager::SCENE_STAGE2:
			m_pScene = CStage3::Create();
			break;
		case CScene_Manager::SCENE_BOSS:
			m_pScene = CStageBoss::Create();
			break;

		case CScene_Manager::END:
			break;
		default:
			break;
		}
		m_eCurScene = m_eNextScene; 
	}
	return S_OK;
}

void CScene_Manager::Update_Scene_Manager()
{
	m_pScene->Update_Scene(); 
}

void CScene_Manager::Render_Scene_Manager()
{
	m_pScene->Render_Scene(); 
}

void CScene_Manager::Release_Scene_Manager()
{
	Safe_Delete(m_pScene); 
}
