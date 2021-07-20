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
#include "Bottle.h"
#include "Time_Manager.h"
#include "Beam.h"

#include "Jet_Normal.h"
#include "Jet_BulletSplash.h"
#include "Jet_Homming.h"
#include "Jet_Rocket.h"
#include "Jet_Rush.h"
#include "Jet_Sniper.h"

#include "Particle.h"
#include "Serpent.h"

IMPLEMENT_SINGLETON(CSpawn_Manager)
list<_vec3> CSpawn_Manager:: m_listPos;
list<MULTIPLACE*> CSpawn_Manager::m_listWaitPlacement;

CSpawn_Manager::CSpawn_Manager(): m_fWaitSpeed(1.f)
{

}


CSpawn_Manager::~CSpawn_Manager()
{
	for (auto& iter : m_listWaitPlacement)
		Safe_Delete(iter);
}

//UI
HRESULT CSpawn_Manager::Spawn(const wstring _wstrObjName, const PLACEMENT * _pPlacement, const OBJECTINFO * _pObjectInfo)
{
	CGameObject* pObject = nullptr;
	if (_pPlacement)
	{
		switch ((RENDERID::ID)_pPlacement->eRenderID)
		{
		case RENDERID::TRUE_FACT:
		case RENDERID::BACKGROUND:
		case RENDERID::MOVE_BACKGROUND1:
		case RENDERID::MOVE_BACKGROUND2:
		case RENDERID::MOVE_BACKGROUND3:
		case RENDERID::GROUND:
			pObject = CBackGround::Create(_pPlacement);
			pObject->Set_RenderId((RENDERID::ID)_pPlacement->eRenderID);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::BACKGROUND), pObject);
			pObject = nullptr;
			break;
		case RENDERID::UI:
			pObject = CGui::Create(_pPlacement);
			pObject->Set_RenderId((RENDERID::ID)_pPlacement->eRenderID);
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), pObject);
			pObject = nullptr;
			break;


		default:
			break;
		}
	}

	return S_OK;
}

//총알
//wstrname 다 오브젝트로 바꿔서 넘겨주기 전에 검색 한 뒤 넘겨주기 
void CSpawn_Manager::Spawn(const wstring _wstrObjName,_vec3 vPos, float fAngle,_vec3 vSpeed,BULLET::SUBWEAPON eSubState)
{
	CGameObject* pObject = nullptr;
	if (_wstrObjName == L"Beam")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CBeam::Create(pObjInfo, vPos, fAngle, vSpeed, eSubState);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Gundrone")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CRocket::Create(pObjInfo, vPos, fAngle, vSpeed);
		static_cast<CBullet*>(pObject)->Set_SubWeaponState(eSubState);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
}

//파티클 
void CSpawn_Manager::Spawn(EFFECT::TYPE _eEffectType, _vec3 vPos, float fAngle, _vec3 vSpeed)
{
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	CGameObject* pParticle = CParticle::Create(pObjInfo, vPos, fAngle, vSpeed,_eEffectType);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pParticle);
	pParticle = nullptr;
}

void CSpawn_Manager::Spawn(const wstring _wstrObjName, _vec3 vPos, float fAngle, _vec3 vSpeed)
{
	CGameObject* pObject = nullptr;
	if (_wstrObjName == L"Minigun")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CNormal::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"EnemyBullet")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CNormal::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Serpent_Bullet")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CNormal::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Rocket")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CRocket::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Rocket_alt")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CRocket::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
	if (_wstrObjName == L"Bottle")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CBottle::Create(pObjInfo, vPos, fAngle, _vec3(vSpeed.x, vSpeed.y - 100.f, 0.f)*0.5f);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;

		pObject = CBottle::Create(pObjInfo, vPos, fAngle, _vec3(vSpeed.x, vSpeed.y + 100.f, 0.f)*0.5f);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;

		pObject = CBottle::Create(pObjInfo, vPos, fAngle, vSpeed);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
		pObject = nullptr;
	}
}




//이펙트
void CSpawn_Manager::Spawn(EFFECT::TYPE _eEffectType, _vec3 vPos,bool _FrameStart)
{
	CGameObject* pEffect = nullptr;

	pEffect = CEffect::Create(_eEffectType,vPos, _FrameStart);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pEffect);
	pEffect = nullptr;
}

void CSpawn_Manager::Spawn(EFFECT::TYPE _eEffectType, _vec3 vPos, bool _FrameStart,float _fSize)
{
	CGameObject* pEffect = nullptr;
	pEffect = CEffect::Create(_eEffectType, vPos, _FrameStart,_fSize);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pEffect);
	pEffect = nullptr;
}

void CSpawn_Manager::Spawn(const wstring _wstrObjName, _vec3 vPos, float fAngle, _vec3 vSpeed, CGameObject * pTarget)
{
	CGameObject* pObject = nullptr;
	if (_wstrObjName == L"Boss_Beam")
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Beam");
		pObject = CBeam::Create(pObjInfo, vPos, fAngle, vSpeed, pTarget,true);
		static_cast<CBeam*>(pObject)->Set_BossBeam(true);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY_BULLET), pObject);
		pObject = nullptr;
	}
}



void CSpawn_Manager::Spawn(const wstring _wstrObjName, _vec3 vPos)
{
	CGameObject* pObject = nullptr;

	if (L"Player" == _wstrObjName)
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CJet_Normal::Create(pObjInfo, vPos);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER), pObject);
		pObject = nullptr;
	}
	if (L"Jet_Normal" == _wstrObjName)
	{
		const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
		pObject = CJet_Normal::Create(pObjInfo, vPos);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), pObject);
		pObject = nullptr;
	}
	if (L"Dead_Explosion1"==_wstrObjName)
	{
		MULTIPLACE* pMultiPlaceInfo = new MULTIPLACE(*CPrefab_Manager::Get_Instance()->Get_MultiPlacementPrefab(_wstrObjName));
		m_listWaitPlacement.emplace_back(pMultiPlaceInfo);
		m_listPos.emplace_back(vPos);
	}
	if (L"Rocket_Explosion" == _wstrObjName)
	{
		MULTIPLACE* pMultiPlaceInfo = new MULTIPLACE(*CPrefab_Manager::Get_Instance()->Get_MultiPlacementPrefab(_wstrObjName));
		m_listWaitPlacement.emplace_back(pMultiPlaceInfo);
		m_listPos.emplace_back(vPos);
	}
	if (L"EffectExplosion_smoke" == _wstrObjName)
	{
		const ANIMATION* pObjInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(_wstrObjName);
		pObject = CEffect::Create(pObjInfo, vPos);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), pObject);
		pObject = nullptr;
	}
	//if (L"Jet_Rocket" == _wstrObjName)
	//{
	//	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
	//	pObject = CJet_Rocket::Create(pObjInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
	//	pObject = nullptr;
	//}
	//if (L"Jet_Rush" == _wstrObjName)
	//{
	//	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
	//	pObject = CJet_Rush::Create(pObjInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
	//	pObject = nullptr;
	//}
	//if (L"Jet_BulletSplash" == _wstrObjName)
	//{
	//	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
	//	pObject = CJet_BulletSplash::Create(pObjInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
	//	pObject = nullptr;
	//}
	//if (L"Jet_Snuper" == _wstrObjName)
	//{
	//	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
	//	pObject = CJet_Snuper::Create(pObjInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
	//	pObject = nullptr;
	//}
	//if (L"Jet_Homming" == _wstrObjName)
	//{
	//	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_wstrObjName);
	//	pObject = CJet_Homming::Create(pObjInfo, vPos);
	//	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::PLAYER_BULLET), pObject);
	//	pObject = nullptr;
	//}

}



void CSpawn_Manager::Update_MultiSpawn()
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

