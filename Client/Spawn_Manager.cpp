#include "stdafx.h"
#include "Spawn_Manager.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"
#include "BackGround.h"
#include "Gui.h"
#include "Prefab_Manager.h"
#include "Normal.h"
#include "Rocket.h"
#include "Time_Manager.h"



IMPLEMENT_SINGLETON(CSpawn_Manager)
list<_vec3> CSpawn_Manager:: m_listPos;
list<MULTIPLACE*> CSpawn_Manager::m_listWaitPlacement;

CSpawn_Manager::CSpawn_Manager(): m_fWaitSpeed(1.f)
{

}


CSpawn_Manager::~CSpawn_Manager()
{
}

//UI
HRESULT CSpawn_Manager::Spawn(const wstring _wstrObjName, const PLACEMENT * _pPlacement, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pObject = nullptr;
	if (_pPlacement)
	{
		switch ((RENDERID::ID)_pPlacement->eRenderID)
		{
		case RENDERID::BACKGROUND:
			pObject = CBackGround::Create(_pPlacement);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::BACKGROUND), pObject);
			pObject = nullptr;
			break;
		case RENDERID::UI:
			pObject = CGui::Create(_pPlacement);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), pObject);
			pObject = nullptr;
			break;
		default:
			break;
		}
	}

	return S_OK;
}

//ÃÑ¾Ë
void CSpawn_Manager::Spawn(const wstring _wstrObjName,_vec3 vPos, float fAngle,_vec3 vSpeed)
{
	CGameObject* pObject = nullptr;
	if (_wstrObjName == L"Minigun")
	{
		const OBJECTINFO* pObjInfo= CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CNormal::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Rocket")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CRocket::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
}

//ÀÌÆåÆ®
void CSpawn_Manager::Spawn(const wstring _wstrObjName, _vec3 vPos,bool _FrameStart,EFFECT::TYPE _eEffectType)
{
	CGameObject* pEffect = nullptr;
	const ANIMATION* pAnimationInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(_wstrObjName);
	pEffect = CEffect::Create(pAnimationInfo, vPos, _FrameStart,_eEffectType);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pEffect);
	pEffect = nullptr;

}

void CSpawn_Manager::Spawn(const wstring _wstrObjName, _vec3 vPos)
{
	CGameObject* pEffect = nullptr;
	MULTIPLACE* pMultiPlaceInfo = new MULTIPLACE(*CPrefab_Manager::Get_Instance()->Get_MultiPlacementPrefab(_wstrObjName));
	m_listWaitPlacement.emplace_back(pMultiPlaceInfo);
	m_listPos.emplace_back(vPos);

	//for (auto& iter : pMultiPlaceInfo->listPlace)
	//{
	//	const PLACEMENT* pPlacementInfo = CPrefab_Manager::Get_Instance()->Get_PlacementPrefab(iter->wstrName);
	//	pEffect = CEffect::Create(pPlacementInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pEffect);
	//	pEffect = nullptr;
	//}
}

void CSpawn_Manager::Update_Spawn()
{
	if (m_listWaitPlacement.empty())
		return;
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fWaitSpeed += fTime;
	if (m_fWaitSpeed > 0.1f)
	{
		m_fWaitSpeed = 0.f;
		auto& iter = m_listWaitPlacement.begin();
		auto& iter_pos = m_listPos.begin();

		for (; iter != m_listWaitPlacement.end();)
		{
			CGameObject* pEffect = nullptr;
			const PLACEMENT* pPlacementInfo = CPrefab_Manager::Get_Instance()->Get_PlacementPrefab((*iter)->listPlace.front()->wstrName);
			pEffect = CEffect::Create(pPlacementInfo, (*iter_pos));
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pEffect);

			Safe_Delete((*iter)->listPlace.front());
			(*iter)->listPlace.pop_front();

			if ((*iter)->listPlace.empty())
			{
				Safe_Delete((*iter));
				iter = m_listWaitPlacement.erase(iter);
				iter_pos = m_listPos.erase(iter_pos);
			}
			else
			{
				iter++;
				iter_pos++;
			}
		}
	}
}

