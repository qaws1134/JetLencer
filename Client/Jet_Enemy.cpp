#include "stdafx.h"
#include "Jet_Enemy.h"
#include "Burner.h"
#include "Arrow_Offscreen.h"
#include "Spawn_Manager.h"
CJet_Enemy::CJet_Enemy()
	:m_fOffsetAngle(0.f)
	, m_fTargetAngle(0.f)
	,m_ePreJetState(ENEMY::END)
	, m_eJetState(ENEMY::END)
	, m_pArrow_Offscreen(nullptr)
	, m_bZoom(false)
{
}


CJet_Enemy::~CJet_Enemy()
{
}



void CJet_Enemy::DeadEffect()
{
	CSpawn_Manager::Spawn(L"Dead_Explosion1", m_tInfo.vPos);
	m_pBurner->Set_Dead(true);
	m_pArrow_Offscreen->Set_Dead(true);
	m_pBurner = nullptr;
	m_bDead = true;
}

void CJet_Enemy::Ui_DistanseState()
{
	switch (m_eUiState)
	{
	case ARROW::DISTANS_FAR:
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_ArrowState(ARROW::DISTANS_FAR);
		break;
	case ARROW::DISTANS_LEVEL0:
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_ArrowState(ARROW::DISTANS_LEVEL0);
		break;
	case ARROW::DISTANS_LEVEL1:
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_ArrowState(ARROW::DISTANS_LEVEL1);	
		break;
	case ARROW::DISTANS_LEVEL2:
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_ArrowState(ARROW::DISTANS_LEVEL2);
		break;
	case ARROW::END:
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_ArrowState(ARROW::END);
		break;
	}
	static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Set_Distance(D3DXVec3Length(&(m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos)));
}

void CJet_Enemy::Ui_DirState()
{
	_vec3 vScroll =CScroll_Manager::Get_Scroll();

	//플레이어에서 몬스터 방향 
	_vec3 vRevDir = m_tInfo.vPos- m_pTarget->Get_ObjInfo().vPos;
	D3DXVec3Normalize(&vRevDir, &vRevDir);

	float fCos = D3DXVec3Dot(&vRevDir, &_vec3(1.f, 0.f, 0.f));

	//플레이어에서 몬스터 앵글 
	float fUiSetAngle = D3DXToDegree(acosf(fCos));
	
	if (vRevDir.y > 0)
		fUiSetAngle = 360.f - fUiSetAngle;
	

	if (fUiSetAngle > 330.f || fUiSetAngle < 30.f)
	{
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Select_Frame(1.f);
		m_pArrow_Offscreen->Set_Pos(_vec3(WINCX-20.f, m_tInfo.vPos.y+ vScroll.y, 0.f));
	}
	else if (fUiSetAngle > 210.f)
	{
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Select_Frame(3.f);
		m_pArrow_Offscreen->Set_Pos(_vec3(m_tInfo.vPos.x+ vScroll.x, WINCY-20.f, 0.f));
	}
	else if (fUiSetAngle > 150.f)
	{
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Select_Frame(0.f);
		m_pArrow_Offscreen->Set_Pos(_vec3(20.f, m_tInfo.vPos.y+ vScroll.y, 0.f));
	}
	else if (fUiSetAngle > 30.f)
	{
		static_cast<CArrow_Offscreen*>(m_pArrow_Offscreen)->Select_Frame(2.f);
		m_pArrow_Offscreen->Set_Pos(_vec3(m_tInfo.vPos.x+ vScroll.x, 20.f, 0.f));
	}
	
}

void CJet_Enemy::EnemyBurnerSet()
{
	static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
	m_pBurner->Set_Pos(m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.7f));
	m_pBurner->Set_Angle(m_fAngle);
}

int CJet_Enemy::Update_GameObject()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fColorTime += fTime;
	if (m_fColorTime > m_fColorSpeed)
	{
		m_tColor = { 255,255,255,255 };
	}
	if (m_bDeadEffect)
		DeadEffect();
	if (m_bDead)
		return OBJ_DEAD;
	if (!m_pTexInfo)
		return OBJ_NOEVENT;
	m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();
	Ui_DistanseState();
	Ui_DirState();

	State_Change();
	TimeCheck();
	Ai_State();		

	JetAngleCheck();
	EnemyBurnerSet();
	SpawnPtFire();

	return OBJ_NOEVENT;
}
