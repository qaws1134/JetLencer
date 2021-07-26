#include "stdafx.h"
#include "SelectStage.h"
#include "ViewText.h"
#include "Gui.h"
#include "Scene_Manager.h"
#include "GameObject_Manager.h"
#include "SoundMgr.h"
#include "SoundMgr.h"

CSelectStage::CSelectStage() : m_iStageIdx(0), m_bDead(false)
{
}

CSelectStage::~CSelectStage()
{
	Release_Scene();
}
CScene * CSelectStage::Create()
{
	CScene* pInstance = new CSelectStage;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
HRESULT CSelectStage::Ready_Scene()
{


	CSoundMgr::Get_Instance()->PlayBGM(L"StageSel.mp3");
	CGameObject* pObject = nullptr;



	pObject = CGui::Create(UI::STAGEBG);
	pObject->Set_Size(_vec3{ 1.4f,1.4f,0.f });
	pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1),0.f });
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, pObject);

	pObject = CGui::Create(UI::MARKER_SHINE);
	pObject->Set_Size(_vec3{ 13.f,8.f,0.f });
	pObject->Set_Angle(270.f);
	pObject->Set_Color(MATCOLOR{ 255,0,0,0 });
	pObject->Set_Pos(_vec3{ 240.f,float(WINCY >> 1),0.f });
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, pObject);


	m_pPointer = CGui::Create(UI::MARKER_SHINE);
	m_pPointer->Set_Size(_vec3{ 1.7f,8.f,0.f });
	m_pPointer->Set_Angle(270.f);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pPointer);

	for (int i = 0; i < 4; i++)
	{
		pObject = CGui::Create(UI::STAGEICON);
		if (i == 0)
			static_cast<CGui*>(pObject)->Select_Frame(3.f);
		else
			static_cast<CGui*>(pObject)->Select_Frame(float(i - 1));

		pObject->Set_Pos(_vec3{ 90.f,(i*150.f)+100.f,0.f });
		pObject->Set_Size(_vec3{ 1.2f,1.2f,0.f });
		m_vecUi.emplace_back(pObject);
	}


	//글자 포스 맞추기 -> 글자 선택하기 ->스테이지 이동하기 
	pObject = CViewText::Create(L"STAGE 01 :");
	pObject->Set_Pos(_vec3{ m_vecUi[0]->Get_ObjInfo().vPos.x+50.f,m_vecUi[0]->Get_ObjInfo().vPos.y-30.f,0.f });
	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"빈 스테이지");
	pObject->Set_Pos(_vec3{ m_vecUi[0]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[0]->Get_ObjInfo().vPos.y ,0.f });
	m_vecUi.emplace_back(pObject);


	pObject = CViewText::Create(L"STAGE 02 :");
	pObject->Set_Pos(_vec3{ m_vecUi[1]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[1]->Get_ObjInfo().vPos.y - 30.f,0.f });

	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"적 섬멸시키기");
	pObject->Set_Pos(_vec3{ m_vecUi[1]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[1]->Get_ObjInfo().vPos.y ,0.f });
	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"STAGE 03 :");
	pObject->Set_Pos(_vec3{ m_vecUi[2]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[2]->Get_ObjInfo().vPos.y - 30.f,0.f });

	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"버티기");
	pObject->Set_Pos(_vec3{ m_vecUi[2]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[2]->Get_ObjInfo().vPos.y ,0.f });
	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"STAGE BOSS :");
	pObject->Set_Pos(_vec3{ m_vecUi[3]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[3]->Get_ObjInfo().vPos.y - 30.f,0.f });

	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(L"보스 서펜트");
	pObject->Set_Pos(_vec3{ m_vecUi[3]->Get_ObjInfo().vPos.x + 50.f,m_vecUi[3]->Get_ObjInfo().vPos.y ,0.f });

	m_vecUi.emplace_back(pObject);


	for (auto& iter : m_vecUi)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}


	return S_OK;
}

void CSelectStage::Update_Scene()
{
	if (m_bDead)
		return;

	m_pPointer->Set_Pos(_vec3{ m_vecUi[m_iStageIdx]->Get_ObjInfo().vPos });

	Key_Check();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CSelectStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CSelectStage::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CSelectStage::Key_Check()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_W))
	{
		m_iStageIdx--;
		if (m_iStageIdx < 0)
			m_iStageIdx = 0;
		else
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_S))
	{
		m_iStageIdx++;
		if (m_iStageIdx >3)
			m_iStageIdx = 3;
		else
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_SELECT);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Select.mp3", CSoundMgr::UI_SELECT);

		switch (m_iStageIdx)
		{
		case 0:
			CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE_EMPTY);
			m_bDead = true;
			break;
		case 1:
			CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE1);
			m_bDead = true;
			break;
		case 2:
			CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE2);
			m_bDead = true;
			break;
		case 3:
			CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_BOSS);
			m_bDead = true;
			break;
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_SELECT);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Select.mp3", CSoundMgr::UI_SELECT);

		CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
		m_bDead = true;
	}


}
