#include "stdafx.h"
#include "MainApp.h"
#include "Player.h"
#include "Scene_Manager.h"
#include "Prefab_Manager.h"
CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
	Release_MainApp();
}

HRESULT CMainApp::Ready_MainApp()
{
	// 	E_FAIL; 
	// 	S_OK; 
	CTime_Manager::Get_Instance()->Ready_Time_Manager();

	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
		goto ERR;

	if (FAILED(CPrefab_Manager::Get_Instance()->Ready_Prefab_Manager()))
		goto ERR;

	CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_STAGE);
	return S_OK;

ERR:
	MessageBox(nullptr, L"Ready by MainApp", L"SystemError", MB_OK);
	return E_FAIL;
}

void CMainApp::Update_MainApp()
{
	CKey_Manager::Get_Instance()->Key_Update();
	CTime_Manager::Get_Instance()->Update_Time_Manager();
	CScene_Manager::Get_Instance()->Update_Scene_Manager();
}

void CMainApp::Render_MainApp()
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	CScene_Manager::Get_Instance()->Render_Scene_Manager();
	CGraphic_Device::Get_Instance()->Render_End();
}

void CMainApp::Release_MainApp()
{
	CKey_Manager::Destroy_Instance();
	CTime_Manager::Destroy_Instance();
	CGameObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
