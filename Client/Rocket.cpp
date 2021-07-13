#include "stdafx.h"
#include "Rocket.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
CRocket::CRocket()
{
}


CRocket::~CRocket()
{
}


CGameObject * CRocket::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed)
{
	CGameObject* pInstance = new CRocket;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	static_cast<CBullet*>(pInstance)->Set_Type((BULLET::TYPE)_tObjectInfo->eBulletType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}



HRESULT CRocket::Ready_GameObject()
{
	m_tInfo.vSize = { 1.3f,1.3f,0.f };
	m_vVelocity = { 0.f,0.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fAccel = 3000.f;
	m_vecCollider.reserve(1);
	
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		m_tCombatInfo.iAtk = 5;
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 13.f, COLLIDER::PLAYER_BULLET));
	}
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		m_tCombatInfo.iAtk = 2;
		m_tCombatInfo.iHp = 3;
		m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 5.f, COLLIDER::ENEMY_BULLET));
	}


	return S_OK;
}

void CRocket::State_Change()
{
	if(m_pObjectInfo->eBulletType == BULLET::GUIDE)
		m_tInfo.vDir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
	if(m_pObjectInfo->eBulletType == BULLET::NORMAL)
		m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
}

void CRocket::Move()
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_vVelocity += m_tInfo.vDir*m_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&m_vVelocity);
	if (fVelLength > 1200.f)
	{
		m_vVelocity *= 1200.f / fVelLength;
	}
	m_tInfo.vPos += m_vVelocity*fTime;
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	for (int i = 0; i < 10; i++)
	{
		if (!m_pTexInfo)
			return;
		CSpawn_Manager::Spawn(EFFECT::ROCKET_PTFIRE, m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.5f) - (vNormalVel*((float)i)*1.5f), false);
	}
}

void CRocket::DeadEffect()
{
	//if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	//{
	//	CSpawn_Manager::Spawn(L"Rocket_Explosion", m_tInfo.vPos);
	//}
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		CSpawn_Manager::Spawn(L"EffectExplosion_smoke", m_tInfo.vPos);

		for (int i = 0; i < rand() % 5; i++)
		{
			float fAngle = (float)(rand() % 360);
			_vec3 Dir = { cosf(D3DXToRadian(fAngle)) ,-sinf(D3DXToRadian(fAngle)) ,0.f };
			float fRad = 100.f;
			CSpawn_Manager::Spawn(EFFECT::ROCKET_BOOM_PTFIRE, m_tInfo.vPos + Dir*fRad, fAngle , m_vVelocity);
		}
		m_bDead = true;
	}
}
