#include "stdafx.h"
#include "Normal.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "ColSphere.h"
#include "Spawn_Manager.h"
#include "SoundMgr.h"
CNormal::CNormal() :m_bDron(false), m_bSound(false)
{
}


CNormal::~CNormal()
{
}


CGameObject * CNormal::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle,_vec3 _vSpeed)
{
	CGameObject* pInstance = new CNormal;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}



HRESULT CNormal::Ready_GameObject()
{
	m_tInfo.vSize = { 0.5f,0.5f,0.f };
	//m_tInfo.vDir.z = 0.f;
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_tFrame.wstrObjKey + m_tFrame.wstrStateKey)->iMax_Index;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fSpeed = 3.f;
	m_vecCollider.reserve(1);

	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		m_tCombatInfo.iAtk = 1;
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 13.f, COLLIDER::PLAYER_BULLET));
	}
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		m_tCombatInfo.iAtk = 1;
		m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 13.f, COLLIDER::ENEMY_BULLET));
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ENEMY_NORMAL);
		CSoundMgr::Get_Instance()->PlaySound(L"Enemy_Bullet.mp3", CSoundMgr::ENEMY_NORMAL);
	}
	if (m_tFrame.wstrStateKey == L"Serpent_Bullet")
	{
		m_fRemoveSpeed = 5.f;		
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SERPENT_BULLET);
		CSoundMgr::Get_Instance()->PlaySound(L"Boss_Bullet.mp3", CSoundMgr::SERPENT_BULLET);


	}
	return S_OK;
}

void CNormal::State_Change()
{
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		if (m_tInfo.vSize.x < 1.2f)
			m_tInfo.vSize += _vec3{ 0.085f,0.085f,0.f };
		else
			m_tFrame.fStartFrame = 1;
	}
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		m_tInfo.vSize = { 1.f,1.f,0.f };
	}
}

void CNormal::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x > Map_Width || m_tInfo.vPos.x <0
		|| m_tInfo.vPos.y <0)
		m_bDead = true;
	if (m_tInfo.vPos.y > Map_Height + 70)
		m_bDeadEffect = true;
	State_Change();
}

void CNormal::Move()
{
	if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		if (!m_bAllTrueMod)
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Player_Bullet.wav", CSoundMgr::PLAYER_BULLET);
				m_bSound = true;
			}
		}
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };

		m_tInfo.vDir *= 850.f*fTime;
		if (!m_bDron)
			m_tInfo.vDir += CGameObject_Manager::Get_Instance()->Get_Player()->Get_Velocity()*fTime;
		else
			m_tInfo.vDir += m_vVelocity*fTime;
	}
	else if ((OBJID::ID)m_pObjectInfo->eObjId == OBJID::ENEMY_BULLET)
	{
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };

		if (m_tFrame.wstrStateKey == L"Serpent_Bullet")
		{
			m_tInfo.vDir *= 350.f*fTime;
			m_fRemoveTIme += fTime;
			if (m_fRemoveTIme > m_fRemoveSpeed)
			{
				m_bDead = true;
			}
		}
		else if (m_tFrame.wstrStateKey == L"SniperBullet")
		{
			m_tInfo.vDir *= 2500.f*fTime;
		}
		else
			m_tInfo.vDir *= 850.f*fTime;
	}

	m_tInfo.vPos += m_tInfo.vDir;

}

void CNormal::DeadEffect()
{
	//오브젝트와 충돌이 되면 충돌 시 이펙트 생성
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_tInfo.vPos.y > Map_Height+70)
	{
		CSpawn_Manager::Spawn(EFFECT::GROUND_WATERSPLASH_FAST, _vec3{ m_tInfo.vPos.x , float(Map_Height + 72) ,0.f}, false, 1.0f);
	}
	else
	{
		CSpawn_Manager::Spawn(EFFECT::BULLET_IMPACT,m_tInfo.vPos,false);
	}

	m_bDead = true;
}
