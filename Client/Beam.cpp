#include "stdafx.h"
#include "Beam.h"
#include "Prefab_Manager.h"
#include "GameObject_Manager.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
#include "SerpentObject.h"
#include "Serpent.h"
#include "Effect.h"
CBeam::CBeam() :m_bBoss(false), bColSet(false), m_fColSize(0.f)
{
}


CBeam::~CBeam()
{
}

CGameObject * CBeam::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, BULLET::SUBWEAPON _eSubId)
{
	CGameObject* pInstance = new CBeam;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	static_cast<CBullet*>(pInstance)->Set_SubWeaponState(_eSubId);
	static_cast<CBullet*>(pInstance)->Set_Type((BULLET::TYPE)_tObjectInfo->eBulletType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
CGameObject * CBeam::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, CGameObject* _pTarget, bool _bBoss)
{
	CGameObject* pInstance = new CBeam;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	pInstance->Set_Target(_pTarget);
	static_cast<CBeam*>(pInstance)->Set_BossBeam(_bBoss);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBeam::Ready_GameObject()
{
	m_vVelocity = { 0.f,0.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 3;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_bCenter = true;
	InitState();
	return S_OK;
}
void CBeam::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fStartFrame = 0.f;
	}
}

void CBeam::Late_Update_GameObject()
{
	//if (m_tInfo.vPos.x > Map_Width || m_tInfo.vPos.x <0
	//	|| m_tInfo.vPos.y <0)
	//	m_bDead = true;
	//if (m_tInfo.vPos.y > Map_Height + 70)
	//	m_bDeadEffect = true;
	State_Change();
}
void CBeam::State_Change()
{
	m_fCenterX = 0.f;
	m_fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);


	if (m_bBoss)
	{
		Boss_State();
	}
	else
	{
		m_tInfo.vSize -= _vec3{ 0.f,0.02f,0.f };
		if (m_tInfo.vSize.y < 0)
		{
			m_bDeadEffect = true;
		}
	}
	
}

void CBeam::Move()
{
	if (!m_bBoss)
	{
		m_fAngle = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Angle();
		m_tInfo.vPos = CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vPos;
		m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	}
	else
	{
		m_fAngle = m_pTarget->Get_Angle();
		m_tInfo.vPos = m_pTarget->Get_ObjInfo().vPos;
		m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	}

	int i = 0;
	for (auto& VecCol : m_vecCollider)
	{	
		i++;
		VecCol->Set_OffSetDir(m_tInfo.vDir*(float)(i)*40.f);
	}
}

void CBeam::DeadEffect()
{
	if (!m_vecCollider.empty())
	{
		//for (auto& iter : m_vecCollider)
		//{
		//	iter->Set_Dead(true);
		//}
		//m_vecCollider.clear();
		//m_vecCollider.shrink_to_fit();
	}
	if (m_bBoss)
	{
		for (auto& iter : m_vecDeadEffect)
		{
			iter->Set_Dead(true);
		}
		m_vecDeadEffect.clear();
		m_vecDeadEffect.shrink_to_fit();
	}
	m_bDead = true;
}

void CBeam::Boss_State()
{
	if (!m_bRemove)
	{
		if (m_tInfo.vSize.y < 1.5f)
		{
			Frame_Change();
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			if (fTime != 0)
			{
				m_tInfo.vSize += _vec3{ 0.f,0.01f,0.f };
				if (m_tInfo.vSize.y > 0.8f)
				{
					CGameObject_Manager::Get_Instance()->Set_AllTrueMode(true);
					float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
					m_fColSize += fTime*50.f;
					for (auto&iter : m_vecCollider)
					{
						if (static_cast<CColSphere*>(iter)->Get_ColSphereSize() < 45.f)
							static_cast<CColSphere*>(iter)->Set_CollisionSize(m_fColSize);
					}
				}
			}
		}
		else
		{
			m_tFrame.fStartFrame = 2;
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_fRemoveTIme += fTime;

			if (m_fRemoveTIme > m_fRemoveSpeed)
				m_bRemove = true;
		}
	}
	else
	{
		m_tInfo.vSize -= _vec3{ 0.f,0.015f,0.f };

		Frame_Change();
		for (size_t i = 0; i < m_vecDeadEffect.size(); i++)
		{
			static_cast<CEffect*>(m_vecDeadEffect[i])->Set_FrameStart(true);
			m_vecDeadEffect[i]->Set_Angle(m_fAngle);
			if (i == 0)
				m_vecDeadEffect[i]->Set_Pos(m_tInfo.vPos);
			else
				m_vecDeadEffect[i]->Set_Pos(m_vecDeadEffect[i - 1]->Get_ObjInfo().vPos + m_tInfo.vDir*1000.f);
		}

		if (m_tInfo.vSize.y < 0.4f)
			CGameObject_Manager::Get_Instance()->Set_AllTrueMode(false);
		if (m_tInfo.vSize.y < 0)
		{
			static_cast<CSerpentObject*>(static_cast<CSerpent*>(m_pTarget)->Get_SerpentObj().front())->Set_Attack_End(true);	
			m_bDeadEffect = true;
		}
	}
}

void CBeam::InitState()
{
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	m_vecCollider.reserve(80);

	if (m_bBoss)
	{
		m_tFrame.fStartFrame = 0;
		m_tCombatInfo.iAtk = 1;
		m_tFrame.fFrameSpeed = 10.f;
		m_tInfo.vSize = { 4.f,0.2f,0.f };
		for (int i = 0; i < 64; i++)
			m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*60.f, 0.f, COLLIDER::ENEMY_BULLET_BEAM));
		m_fRemoveSpeed= 3.f;
		m_fRemoveTIme=  0.f;
		m_bRemove = false;
		CScroll_Manager::Shake(20.f, 1.f);
		for (int i = 0; i < 5; i++)
		{
			CGameObject* pObject = CEffect::Create(EFFECT::BOSS_LASER_END);
			m_vecDeadEffect.emplace_back(pObject);
		}
		for (auto& iter : m_vecDeadEffect)
		{
			CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), iter);
		}
	}
	else
	{
		switch (m_eSubWeaponState)
		{
		case BULLET::CHARGE_1:
			m_tFrame.fStartFrame = 0;
			m_tCombatInfo.iAtk = 5;
			for (int i = 0; i < 20; i++)
				m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*40.f, 10.f, COLLIDER::PLAYER_BULLET_BEAM));
			m_tInfo.vSize = { 1.f,1.f,0.f };
			CScroll_Manager::Shake( 8.f, 0.5f);
			break;
		case BULLET::CHARGE_2:
			m_tFrame.fStartFrame = 1;
			m_tCombatInfo.iAtk = 6;
			for (int i = 0; i < 20; i++)
				m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*40.f, 15.f, COLLIDER::PLAYER_BULLET_BEAM));
			m_tInfo.vSize = { 1.f,1.3f,0.f };
			CScroll_Manager::Shake(11.f, 0.5f);
			break;
		case BULLET::CHARGE_3:
			m_tFrame.fStartFrame = 2;
			m_tCombatInfo.iAtk = 7;
			for (int i = 0; i < 20; i++)
				m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*40.f, 20.f, COLLIDER::PLAYER_BULLET_BEAM));
			m_tInfo.vSize = { 1.f,1.8f,0.f };
			CScroll_Manager::Shake(16.f, 0.5f);
			break;
		}

	}
}
