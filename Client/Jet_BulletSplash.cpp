#include "stdafx.h"
#include "Jet_BulletSplash.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
#include "Arrow_Offscreen.h"

CJet_BulletSplash::CJet_BulletSplash()
{
}


CJet_BulletSplash::~CJet_BulletSplash()
{
}

CGameObject * CJet_BulletSplash::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos)
{
	CGameObject* pInstance = new CJet_BulletSplash;
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

HRESULT CJet_BulletSplash::Ready_GameObject()
{
	InitJet();

	m_tCombatInfo.iAtk = 1;
	m_tCombatInfo.iHp = 5;

	m_fAccel = 1800.f;
	m_fMaxSpeed = 500.f;

	m_eJetState = ENEMY::ATTACK;
	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = -22.5f;
	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 30.f, COLLIDER::ENEMY));

	m_pArrow_Offscreen = CArrow_Offscreen::Create(UI::JET);

	return S_OK;
}

void CJet_BulletSplash::State_Change()
{
	if (m_ePreJetState != m_eJetState)
	{
		switch (m_eJetState)
		{
		case ENEMY::ATTACK:
			m_vResultDir = m_pTarget->Get_ObjInfo().vPos-m_tInfo.vPos ;
			break;
		case ENEMY::BACK:
			//m_vResultDir = m_tInfo.vPos - m_pTarget->Get_ObjInfo().vPos;
			break;
		}
		m_ePreJetState = m_eJetState;
	}
}

void CJet_BulletSplash::Ai_State()
{
	m_vTarget_Dir = m_vResultDir;
	TargetAngle_Check();
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	m_bAccel = true;

	if (m_eJetState == ENEMY::ATTACK)
	{
		if (m_eUiState == ARROW::END)
		{
			m_eJetState = ENEMY::BACK;
		}		
		if (m_eUiState != ARROW::END)
		{
			m_vResultDir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
		}
	}
	else
	{
		if (m_eUiState == ARROW::DISTANS_LEVEL1)
		{
			m_eJetState = ENEMY::ATTACK;
		}
	}
}

void CJet_BulletSplash::DeadEffect()
{
	RandomEffect(EFFECT::EXPLOSION1, 3, 60);
	RandomEffect(EFFECT::EXPLOSION2, 3, 60);
	RandomEffect(EFFECT::EXPLOSION3, 3, 60);

	CSpawn_Manager::Spawn(EFFECT::MEGA_EXPLOSION2, m_tInfo.vPos, false);
	CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
	int iMaxBullet = 12;
	for (int i = 0; i < iMaxBullet; i++)
		CSpawn_Manager::Spawn(L"Serpent_Bullet", m_tInfo.vPos, (360.f/iMaxBullet)*i, m_vVelocity);


	m_pBurner->Set_Dead(true);
	m_pArrow_Offscreen->Set_Dead(true);
	m_pBurner = nullptr;
	m_bDead = true;
}
