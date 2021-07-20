#include "stdafx.h"
#include "Rocket.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
CRocket::CRocket() 
	: m_fTargetSpeed(0.f)
	, m_fTargetTime(0.f)
	,m_pTargetUi(nullptr)
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
	m_vVelocity = { 0.f,0.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	
	m_vecCollider.reserve(1);
	
	m_vVelocity = { 1.f,1.f,0.f };
	m_vGravity = { 0.f,1.f,0.f };
	m_fRegistPower = 2000.f;
	m_fAngleSpeed = 220.f;
	m_fAccel = 2500.f;
	m_fMaxSpeed = 650.f;





	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		m_tCombatInfo.iAtk = 5;
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 13.f, COLLIDER::PLAYER_BULLET));
		m_tInfo.vSize = { 1.3f,1.3f,0.f };
	}
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		m_tCombatInfo.iAtk = 2;
		m_tCombatInfo.iHp = 3;
		m_tInfo.vSize = { 1.5f,1.3f,0.f };
		m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 5.f, COLLIDER::ENEMY_BULLET));
	}


	return S_OK;
}

void CRocket::State_Change()
{
	if (m_pObjectInfo->eBulletType == BULLET::GUIDE)
	{
		if (m_pTarget)
			m_vTarget_Dir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
		else
			m_vTarget_Dir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	}
	
	if(m_pObjectInfo->eBulletType == BULLET::NORMAL)
		m_vTarget_Dir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
}

void CRocket::Move()
{
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		if (m_pObjectInfo->eBulletType == BULLET::GUIDE)
		{
			m_pTarget = CGameObject_Manager::Get_Instance()->Get_Target(this, OBJID::ENEMY);
		}
	}
	State_Change();
	TargetAngle_Check();
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
/*
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_vVelocity += m_tInfo.vDir*m_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&m_vVelocity);
	if (fVelLength > 1500.f)
	{
		m_vVelocity *= 1500.f / fVelLength;
	}
	m_tInfo.vPos += m_vVelocity*fTime;
	*/
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	for (int i = 0; i < 10; i++)
	{
		if (!m_pTexInfo)
			return;
		CSpawn_Manager::Spawn(EFFECT::ROCKET_PTFIRE, m_tInfo.vPos -(m_tInfo.vDir*10.f) - vNormalVel*float(i), false);
	}


}

void CRocket::DeadEffect()
{
	CScroll_Manager::Shake(6.f, 0.5f);
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		if (m_tInfo.vPos.y > Map_Height + 70)
		{
			CSpawn_Manager::Spawn(EFFECT::GROUND_WATERSPLASH_FAST, _vec3{ m_tInfo.vPos.x , float(Map_Height + 72) ,0.f }, false, 1.0f);
		}
		else
		{
			CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
			RandomEffect(EFFECT::EXPLOSION1, 3, 60);
			RandomEffect(EFFECT::EXPLOSION2, 3, 60);
			RandomEffect(EFFECT::EXPLOSION3, 3, 60);
		}
		m_bDead = true;
	}


	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		if (m_tInfo.vPos.y > Map_Height + 70)
		{
			CSpawn_Manager::Spawn(EFFECT::GROUND_WATERSPLASH_FAST, _vec3{ m_tInfo.vPos.x , float(Map_Height + 72) ,0.f }, false, 1.0f);
		}
		else
		{
			CSpawn_Manager::Spawn(L"EffectExplosion_smoke", m_tInfo.vPos);
			for (int i = 0; i <  rand()%5+2; i++)
			{
				float fAngle = (float)(rand() % 360);
				_vec3 Dir = { cosf(D3DXToRadian(fAngle)) ,-sinf(D3DXToRadian(fAngle)) ,0.f };
				float fRad = 80.f;
				CSpawn_Manager::Spawn(EFFECT::ROCKET_BOOM_PTFIRE, m_tInfo.vPos + Dir*fRad, fAngle, Dir*3.f);
			}
		}

		m_bDead = true;
	}
}

void CRocket::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x > Map_Width || m_tInfo.vPos.x <0
		|| m_tInfo.vPos.y <0)
		m_bDead = true;
	if (m_tInfo.vPos.y > Map_Height + 70)
		m_bDeadEffect = true;
}


void CRocket::Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity)
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vVec = m_vVelocity + vDir*_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&vVec);

	if (bGravity)
		m_vVelocity += vDir*_fAccel*fTime;

	if (fVelLength > _fMaxSpeed + 30.f)
	{
		float fVelLength = D3DXVec3Length(&m_vVelocity);
		float fRegist = m_fRegistPower * fTime;
		if (fRegist > fVelLength)
			fRegist = fVelLength;
		_vec3 vRegist = (m_vVelocity* -fRegist);
		if (bGravity)
			vRegist.y = 0;
		D3DXVec3Normalize(&vRegist, &vRegist);
		m_vVelocity += vRegist*10.f;
		//최대속도 
	}
	else if (fVelLength > _fMaxSpeed)
		m_vVelocity *= _fMaxSpeed / fVelLength;
	else
	{
		if (!bGravity)
			m_vVelocity += vDir*_fAccel*fTime;
	}

	m_tInfo.vPos += (m_vVelocity*fTime);
}
