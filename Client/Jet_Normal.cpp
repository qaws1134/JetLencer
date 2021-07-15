#include "stdafx.h"
#include "Jet_Normal.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
#include "Arrow_Offscreen.h"
CJet_Normal::CJet_Normal()
{
}


CJet_Normal::~CJet_Normal()
{
}


CGameObject * CJet_Normal::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos)
{
	CGameObject* pInstance = new CJet_Normal;
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

HRESULT CJet_Normal::Ready_GameObject()
{
	InitJet();

	m_tCombatInfo.iAtk = 1;
	m_tCombatInfo.iHp = 5;

	m_fAttackTime = 5.f;
	m_fAttackSpeed = 5.f;

	m_fAccel = 1800.f;
	m_fMaxSpeed = 500.f;

	m_eJetState = ENEMY::ATTACK;
	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = -22.5f;
	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 15.f, COLLIDER::ENEMY));

	m_pArrow_Offscreen = CArrow_Offscreen::Create(UI::JET);
	return S_OK;
}

void CJet_Normal::State_Change()
{

	if (m_ePreJetState!= m_eJetState)
	{
		switch (m_eJetState)
		{
		case ENEMY::ATTACK:

			break;
		case ENEMY::BACK:
			//타겟을 화면 끝 아무지점 
			//컬라이더를 만들어서 컬라이더 안에 들어오면 공격, 공격 끝나면 컬라이더를 벗어나자
			//혹은 컬라이더 내부에서 일정 시간이 지나면 다시 공격 상태로 변경
			m_vResultDir = m_tInfo.vPos - m_pTarget->Get_ObjInfo().vPos;
			break;
		}
		m_ePreJetState = m_eJetState;
	}
}

void CJet_Normal::Ai_State()
{
	
	
	m_vTarget_Dir = m_vResultDir;
	TargetAngle_Check();
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	m_bAccel = true;
	SpawnPtFire();

	if (m_eJetState == ENEMY::ATTACK)
	{
		m_vResultDir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
		
		if (m_eUiState == ARROW::END&& AttackTime())
		{
			CSpawn_Manager::Spawn(L"EnemyBullet", m_tInfo.vPos, m_fAngle, m_vVelocity);
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

	//공격
	//공격하고나서 state 변경 
	//타겟을 플레이어
	//타겟이 자신의 dir방향에서 일정 각도 범위 내에 들어오면 총알 발사 오차는 +-5도정도??
	
	//도주
	//플레이어의 범위 컬라이더를 만들어서 컬라이더 밖은 회피상태 안은 공격상태 
	//공격 끝나면 컬라이더를 벗어나자 -> auto k??
	//
	//혹은 컬라이더 내부에서 일정 시간이 지나면 다시 공격 상태로 변경
}


