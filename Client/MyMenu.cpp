#include "stdafx.h"
#include "MyMenu.h"
#include "ViewText.h"
#include "Gui.h"
#include "Scene_Manager.h"
#include "GameObject_Manager.h"
#include "SoundMgr.h"
CMyMenu::CMyMenu()
{
	m_vecMenuIndex.reserve(8);
	m_vecUi.reserve(32);
	m_vecCloude.reserve(1000);
}


CMyMenu::~CMyMenu()
{
	Release_Scene();
}

CScene * CMyMenu::Create()
{
	CScene* pInstance = new CMyMenu;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CMyMenu::Ready_Scene()
{

	CSoundMgr::Get_Instance()->PlayBGM(L"MainMenu.mp3");

	CGameObject* pObject = nullptr;

	m_iMaxCloude = 1000;
	m_iActionIdx = m_iMaxCloude -1;
	m_bAction = false;
	m_fIndexYOffset = 15.f;

	for (int i = 0; i < m_iMaxCloude; i++)
	{
		pObject = CGui::Create(UI::CLOUDE);
		pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 180.f,0.f });
		static_cast<CGui*>(pObject)->Select_Frame(float(i % 5));
		static_cast<CGui*>(pObject)->Set_Render(false);
		static_cast<CGui*>(pObject)->Set_Action(false);

		m_vecCloude.emplace_back(pObject);

		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, pObject);
	}



	m_fActionTime = 0.f;
	m_fActionSpeed = 0.08f;
	m_iButtonIdx = 0;


	pObject = CGui::Create(UI::MENUBACKGROUND);
	m_vecUi.emplace_back(pObject);

	pObject = CGui::Create(UI::SCREENBLACK);
	pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1),0.f });
	pObject->Set_Size(_vec3{2.f,4.f,0.f});
	pObject->Set_Alpha(150);
	m_vecUi.emplace_back(pObject);

	pObject = CGui::Create(UI::LOGO);
	pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1)-150.f,0.f });
	pObject->Set_Size(_vec3{0.7f,0.7f,0.f });
	m_vecUi.emplace_back(pObject);

	pObject = CGui::Create(UI::BUTTON);
	pObject->Set_Size(_vec3{ 0.7f,0.8f,0.f });
	pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1),0.f });
	static_cast<CGui*>(pObject)->Select_Frame(4.f);
	m_vecUi.emplace_back(pObject);




	pObject = CViewText::Create(_vec3{ float(WINCX >> 1) - 65.f,float(WINCY >> 1) + 100.f,0.f }, L"STAGE SELECT");
	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(_vec3{ float(WINCX >> 1) - 45.f,float(WINCY >> 1) + 150.f,0.f }, L"LOADOUT");
	m_vecUi.emplace_back(pObject);

	pObject = CViewText::Create(_vec3{ float(WINCX >> 1) - 25.f,float(WINCY >> 1) + 200.f,0.f }, L"QUIT");
	m_vecUi.emplace_back(pObject);

	m_vecMenuIndex.emplace_back(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 100.f+ m_fIndexYOffset,0.f });
	m_vecMenuIndex.emplace_back(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 150.f+ m_fIndexYOffset,0.f });
	m_vecMenuIndex.emplace_back(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 200.f+ m_fIndexYOffset,0.f });
	m_vecUi[3]->Set_Pos(m_vecMenuIndex[0]);
	for (auto& iter : m_vecUi)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}

	return S_OK;
}

void CMyMenu::Update_Scene()
{
	if (m_bDead)
		return;
	m_vecUi[3]->Set_Dir(m_vecMenuIndex[m_iButtonIdx]);
	_vec3 vTargetDir = m_vecUi[3]->Get_ObjInfo().vDir - m_vecUi[3]->Get_ObjInfo().vPos;
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);
	m_vecUi[3]->Set_Pos(m_vecUi[3]->Get_ObjInfo().vPos+vTargetDir*5.f);
	
	SpawnCloude();
	Key_Check();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();


}

void CMyMenu::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CMyMenu::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CMyMenu::SpawnCloude()
{

	if (m_vecCloude.empty())
		return;
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fActionTime += fTime;
	if (m_fActionTime > m_fActionSpeed)
	{
		m_fActionTime = 0.f;

		static_cast<CGui*>(m_vecCloude[m_iActionIdx])->Set_Render(true);
		static_cast<CGui*>(m_vecCloude[m_iActionIdx])->Set_Action(true);
		m_iActionIdx--;
		if (m_iActionIdx == 0)
		{
			for (int i = 0; i < m_iMaxCloude; i++)
			{
				CGameObject* pObject = nullptr;
				pObject = CGui::Create(UI::CLOUDE);
				pObject->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 200.f,0.f });
				static_cast<CGui*>(pObject)->Select_Frame(float(i % 5));
				static_cast<CGui*>(pObject)->Set_Render(false);
				static_cast<CGui*>(pObject)->Set_Action(false);

				m_iActionIdx = m_iMaxCloude - 1;
				m_vecCloude.emplace_back(pObject);

				CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, pObject);
			}
		}
	}
}

void CMyMenu::Key_Check()
{

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_W))
	{
		m_iButtonIdx--;
		if (m_iButtonIdx < 0)
			m_iButtonIdx = 0;
		else 
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}

	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_S))
	{
		m_iButtonIdx++;
		if (m_iButtonIdx >2)
			m_iButtonIdx = 2;
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
		//¾ÀÀüÈ¯
		switch (m_iButtonIdx)
		{
		case 0:
			//CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE_SEL);
			m_bDead = true;
			break;
		case 1:
			//·Îµå¾Æ¿ô
			m_bDead = true;
			CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_LOADOUT);
			break;
		case 2:
			//Á¾·á
			break;

		}

	}

}
