#include "stdafx.h"
#include "SerpentHead.h"
#include "Spawn_Manager.h"
#include "Collider.h"

CSerpentHead::CSerpentHead()
{
}


CSerpentHead::~CSerpentHead()
{
}

CGameObject* CSerpentHead::Create(const OBJECTINFO * _tObjectInfo)
{
	CGameObject* pInstance = new CSerpentHead;
	pInstance->Set_Prefab(_tObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CSerpentHead::Ready_GameObject()
{
	InitJet();
	return S_OK;
}

void CSerpentHead::Ai_State()
{

	Select_Armor();
	Move();

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bAttack)
	{
		Animation_Change(m_AniCrash);
		CSpawn_Manager::Spawn(L"Boss_Beam", m_tInfo.vPos - m_pTarget->Get_Velocity()*fTime, m_fAngle, m_pTarget->Get_Velocity(),m_pTarget);
		m_bAttack = false;
	}

}
void CSerpentHead::State_Change()
{
	if (!m_bStart)
	{
		switch (m_eSerpentPattern)
		{
		case SERPENT::RAGER:
			m_bAttack = true;
			m_bStart = true;
			break;
		case SERPENT ::END:
			break;
		}
	}
}

void CSerpentHead::Select_Armor()
{
	if (7 < m_vecCollider.front()->Get_CombatInfo().iHp)
	{
		Animation_Change(m_AniIdle);
	}
	else if (3 < m_vecCollider.front()->Get_CombatInfo().iHp)
	{
		Animation_Change(m_AniFrame);
	}
	else
	{
		Animation_Change(m_AniFrame);
		m_bCrash = true;
	}
	
}

