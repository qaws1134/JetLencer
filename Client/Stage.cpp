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
#include "SoundMgr.h"

CStage::CStage():m_iStageUiIndex(0),m_fStartSpeed(0.f),m_fStartTime(0.f)
{
	m_vecStageStart.reserve(10);
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

	CPrefab_Manager::Get_Instance()->SpawnObjectbyScene(CScene_Manager::SCENE_STAGE_EMPTY);

	CGameObject* pObject = nullptr;

	pObject = CPlayer::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::PLAYER, pObject);

	pObject = CMouse::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::MOUSE, pObject);
	
	

	m_fStartSpeed = 0.5f;

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
		iter->Set_Pos(_vec3{ float(WINCX>>1),float(WINCY>>1)-200.f,0.f });
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}
	return S_OK;
}

void CStage::Update_Scene()
{

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CSpawn_Manager::Destroy_Instance();
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		CGameObject_Manager::Get_Instance()->Release_GameObject_Manager();
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
		return;
	}


	//if (m_iStageUiIndex < 4)
	//{
	//	CSoundMgr::Get_Instance()->PlaySound(L"Stage_CountDown.wav", CSoundMgr::STAGE_COUNT);
	//	static_cast<CGui*>(m_vecStageStart[m_iStageUiIndex])->Set_Action(true);
	//	if (static_cast<CGui*>(m_vecStageStart[m_iStageUiIndex])->Get_Start())
	//	{
	//		m_iStageUiIndex++;
	//	}
	//}
	//else
	//{
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Start(true);
	//}
	



	if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM1))
	{
		CSpawn_Manager::Spawn(L"Jet_Normal", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM2))
	{
		CSpawn_Manager::Spawn(L"Jet_Rocket", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM3))
	{
		CSpawn_Manager::Spawn(L"Jet_BulletSplash", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM4))
	{
		CSpawn_Manager::Spawn(L"Jet_Sniper", _vec3{ 400.f + float(Map_Width >> 1),200.f + float(Map_Height >> 1),0.f });
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
