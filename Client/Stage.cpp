#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Prefab_Manager.h"
#include "Player.h"
#include "Mouse.h"
#include "Spawn_Manager.h"
CStage::CStage()
{
}


CStage::~CStage()
{
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



	return S_OK;
}

void CStage::Update_Scene()
{
	CSpawn_Manager::Get_Instance()->Update_Spawn();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager(); 
}

void CStage::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CStage::Release_Scene()
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
