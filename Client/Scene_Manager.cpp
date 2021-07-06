#include "stdafx.h"
#include "Scene_Manager.h"
#include "Stage.h"
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
		case CScene_Manager::SCENE_LOADING:
			break;
		case CScene_Manager::SCENE_STAGE:
			m_pScene = CStage::Create(); 
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
