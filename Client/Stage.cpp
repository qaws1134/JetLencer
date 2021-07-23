#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "Mouse.h"
#include "Spawn_Manager.h"
#include "Scroll_Manager.h"
#include "ViewText.h"
#include "Arrow_Offscreen.h"
#include "Serpent.h"

CStage::CStage()
{

}


CStage::~CStage()
{

}

CScene * CStage::Create()
{
	CScene* pInstance = new CStage;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CStage::Ready_Scene()
{
	//pObject =	CTerrain::Create();
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJECTINFO::BACKGROUND, pObject);
	//pObject = nullptr
	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::SCENE_STAGE);


	

	CGameObject* pObject = nullptr;



	pObject = CPlayer::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::PLAYER, pObject);

	pObject = CMouse::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::MOUSE, pObject);

	//m_pOject1 = CViewText::Create(_vec3{ 100.f,100.f,0.f }, L"scrollX %f");
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pOject1);
	//m_pOject2 = CViewText::Create(_vec3{ 100.f,200.f,0.f }, L"scrollY %f");
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pOject2);

	//m_pUI3 = CArrow_Offscreen::Create(UI::JET);
	//CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pUI3);



	//CSpawn_Manager::Spawn(L"Jet_Normal", _vec3{ 400.f+float(Map_Width>>1),100.f+ float(Map_Height>>1),0.f });
	//CSpawn_Manager::Spawn(L"Jet_Normal", _vec3{ 400.f+ float(Map_Width>>1),200.f+ float(Map_Height>>1),0.f });

	return S_OK;
}

void CStage::Update_Scene()
{


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
	
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
	}


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_P))
	{
		//º¸½º »Ç¤³±â
		CGameObject*pObject = nullptr;
		pObject = CSerpent::Create(_vec3{ 400.f + float(Map_Width >> 1),100.f + float(Map_Height >> 1),0.f });
		//CSpawn_Manager::Spawn(L"Jet_Normal", _vec3{ 400.f + float(Map_Width >> 1),100.f + float(Map_Height >> 1),0.f });
		//CSpawn_Manager::Spawn(L"Jet_Normal", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
		//CSpawn_Manager::Spawn(L"Jet_Rocket", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
	}

	CSpawn_Manager::Get_Instance()->Update_MultiSpawn();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
	CScroll_Manager::Scroll_Lock();
}

void CStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage::Release_Scene()
{
	CSpawn_Manager::Destroy_Instance();
}
