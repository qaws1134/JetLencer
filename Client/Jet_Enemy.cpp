#include "stdafx.h"
#include "Jet_Enemy.h"
#include "Burner.h"

CJet_Enemy::CJet_Enemy()
	:m_fOffsetAngle(0.f)
	, m_fTargetAngle(0.f)
{
}


CJet_Enemy::~CJet_Enemy()
{
}



void CJet_Enemy::DeadEffect()
{
}

int CJet_Enemy::Update_GameObject()
{
	m_tColor = { 255,255,255,255 };
	if (m_bDeadEffect)
	{
		DeadEffect();
	}
	if (m_bDead)
	{
		return OBJ_DEAD;
	}
	if (!m_pTexInfo)
		return OBJ_NOEVENT;
	m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();
	State_Change();
	TimeCheck();
	//Ai_State();						
	JetAngleCheck();
	static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
	m_pBurner->Set_Pos(m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.7f));
	m_pBurner->Set_Angle(m_fAngle);
	SpawnPtFire();
	//m_fAngle -= m_fAngleOffset*0.5f;
	//BurnerState();	//버너 상태 체크

	return OBJ_NOEVENT;
}
