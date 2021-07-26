#include "stdafx.h"
#include "Jet_Rocket.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
#include "Arrow_Offscreen.h"

CJet_Rocket::CJet_Rocket()
{
}


CJet_Rocket::~CJet_Rocket()
{

}
CGameObject * CJet_Rocket::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos)
{
	CGameObject* pInstance = new CJet_Rocket;
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

HRESULT CJet_Rocket::Ready_GameObject()
{
	InitJet();

	m_tCombatInfo.iAtk = 1;
	m_tCombatInfo.iHp = 5;

	m_fAttackTime = 5.f;
	m_fAttackSpeed = float(rand()%4+5);

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

void CJet_Rocket::State_Change()
{
	if (m_ePreJetState != m_eJetState)
	{
		switch (m_eJetState)
		{
		case ENEMY::ATTACK:

			break;
		case ENEMY::BACK:
			m_vResultDir = m_tInfo.vPos - m_pTarget->Get_ObjInfo().vPos;
			break;
		}
		m_ePreJetState = m_eJetState;
	}
}

void CJet_Rocket::Ai_State()
{
	m_vTarget_Dir = m_vResultDir;
	TargetAngle_Check();
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	m_bAccel = true;

	if (m_eJetState == ENEMY::ATTACK)
	{
		m_vResultDir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;

		if ((m_eUiState == ARROW::DISTANS_LEVEL2 || m_eUiState == ARROW::END) && AttackTime())
		{
			CSpawn_Manager::Spawn(L"EnemyRocket", m_tInfo.vPos, m_fAngle, m_vVelocity);
			m_eJetState = ENEMY::BACK;
		}
	}
	else
	{
		if (m_eUiState == ARROW::DISTANS_LEVEL1
			|| AttackTime())
		{
			m_eJetState = ENEMY::ATTACK;
		}
	}
}
