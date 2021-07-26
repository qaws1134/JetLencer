#include "stdafx.h"
#include "Jet_Sniper.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
#include "Arrow_Offscreen.h"
#include "Effect.h"
#include "Particle.h"
#include "SoundMgr.h"
CJet_Sniper::CJet_Sniper() : m_bAttack(false)
{
	m_vecArrow.reserve(30);
}


CJet_Sniper::~CJet_Sniper()
{
	m_vecArrow.swap(vector<CGameObject*>());
	m_vecArrow.clear();
}

CGameObject * CJet_Sniper::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos)
{
	CGameObject* pInstance = new CJet_Sniper;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CJet_Sniper::Ready_GameObject()
{

	InitJet();

	m_tCombatInfo.iAtk = 1;
	m_tCombatInfo.iHp = 12;

	m_fAttackTime = 7.f;
	m_fAttackSpeed =3.f;
	m_fRegistPower = 10000.f;

	m_fAccel = 1800.f;
	m_fMaxSpeed = 10.f;
	m_fAngleSpeed = 60.f;
	m_eJetState = ENEMY::ATTACK;
	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = -22.5f;
	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 30.f, COLLIDER::ENEMY));

	m_pArrow_Offscreen = CArrow_Offscreen::Create(UI::TARGET);

	for (int i = 0; i < 30; i++)
	{
		CGameObject* pObject = CEffect::Create(EFFECT::SNIPER_ARROW, m_tInfo.vPos);
		static_cast<CEffect*>(pObject)->Set_OffsetDir(m_tInfo.vDir*(float)(i)*40.f);
		m_vecArrow.emplace_back(pObject);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::EFFECT, pObject);
	}
	return S_OK;
}

void CJet_Sniper::State_Change()
{

		switch (m_eJetState)
		{
		case ENEMY::ATTACK:
			m_fMaxSpeed = 10.f;
			m_vResultDir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
			break;
		case ENEMY::BACK:
			m_fMaxSpeed = 300.f;
			m_vResultDir =  m_pTarget->Get_ObjInfo().vPos- m_tInfo.vPos;
			break;
		}

}

void CJet_Sniper::Ai_State()
{
	m_vTarget_Dir = m_vResultDir;
	TargetAngle_Check();
	m_bAccel = true;
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	if (m_eJetState == ENEMY::ATTACK)
	{
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_fAttackTime += fTime;

		//´ë±â

		if (m_fAttackTime > m_fAttackSpeed)
		{
			m_bAttack = true;
			if (!static_cast<CEffect*>(m_vecArrow.front())->Get_End())
			{
				for (size_t i = 0; i < m_vecArrow.size(); i++)
				{
					m_vecArrow[i]->Set_Angle(m_fAngle);
					static_cast<CEffect*>(m_vecArrow[i])->Set_FrameStart(true);
					m_vecArrow[i]->Set_Pos(m_tInfo.vPos + (m_tInfo.vDir*(float)(i)*120.f));
				}
			}
			else if (static_cast<CEffect*>(m_vecArrow.front())->Get_End())
			{
				if (m_bAttack)
				{
					CSpawn_Manager::Spawn(L"EnemySniperBullet", m_tInfo.vPos, m_fAngle, m_vVelocity);
					m_fAttackTime = 0.f;
					m_bAttack = false;
					static_cast<CEffect*>(m_vecArrow.front())->Set_End(false);
				}
			}
		}
		if (m_eUiState < ARROW::DISTANS_LEVEL1)
		{

			m_eJetState = ENEMY::BACK;
		}
	}
	else
	{
		if (m_eUiState > ARROW::DISTANS_LEVEL1)
		{
			m_eJetState = ENEMY::ATTACK;
		}

	}
}

void CJet_Sniper::DeadEffect()
{
	CGameObject* pObject = nullptr;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT_BOOM2);
	CSoundMgr::Get_Instance()->PlaySound(L"Explosion2.mp3", CSoundMgr::EFFECT_BOOM2);

	pObject = CParticle::Create(m_pObjectInfo, m_tInfo.vPos, m_fAngle, m_vVelocity, EFFECT::JET_PTFIRE_RED);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::EFFECT, pObject);

	CSpawn_Manager::Spawn(EFFECT::MEGA_EXPLOSION3, m_tInfo.vPos, false);
	RandomEffect(EFFECT::EXPLOSION1, 3, 60);
	RandomEffect(EFFECT::EXPLOSION2, 3, 60);
	RandomEffect(EFFECT::EXPLOSION3, 3, 60);
	CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
	m_pBurner->Set_Dead(true);
	m_pArrow_Offscreen->Set_Dead(true);
	m_pBurner = nullptr;
	m_bDead = true;
	for (auto& iter : m_vecArrow)
		iter->Set_Dead(true);
	m_vecArrow.swap(vector<CGameObject*>());
	m_vecArrow.clear();
}
