#include "stdafx.h"
#include "SerpentTail.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"
#include "Collider.h"

CSerpentTail::CSerpentTail() : m_bTriple(false),m_iAttackCount(0)
{
}


CSerpentTail::~CSerpentTail()
{
}

CGameObject* CSerpentTail::Create(const OBJECTINFO * _tObjectInfo)
{
	CGameObject* pInstance = new CSerpentTail;
	pInstance->Set_Prefab(_tObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CSerpentTail::Ready_GameObject()
{
	InitJet();

	return S_OK;
}

void CSerpentTail::Ai_State()
{
	Select_Armor();
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	Move();
	if (m_bAttack)
	{
		if (!m_bAttack_End)
		{
			//공격 시간 
			m_fAnimationStart += fTime;
			if (m_fAnimationStart > m_fAnimationSpeed)
			{

				if (!m_bTriple)
				{
					float fAngle = m_fAngle - 180.f;
					if (fAngle < 0.f)
						fAngle += 360.f;
					CSpawn_Manager::Spawn(L"EnemyBullet", m_tInfo.vPos, fAngle,(m_tInfo.vDir*0.01f));
				}
				else
				{
					float fAngle = m_fAngle - 195.f;
					if (fAngle < 0.f)
						fAngle += 360.f;
					for (int i = 0; i < 3; i++)
					{
						CSpawn_Manager::Spawn(L"EnemyBullet", m_tInfo.vPos, fAngle, (m_tInfo.vDir*0.5f));
						fAngle += 15.f;
						if (fAngle > 360.f)
						{
							fAngle -= 360.f;
						}
					}
				}

				m_fAnimationStart = 0.f;
				m_iAttackCount++;
				if (m_iAttackCount == 5)
				{
					m_bAttack_End = true;
					m_fPatternTime = 0.f;
					m_iAttackCount = 0;
				}
			}
		}
		else
		{
			//패턴 시간 
			m_fPatternTime += fTime;
			if (m_fPatternTime > m_fPatternSpeed)
			{
				m_bAttack_End = false;
				m_fPatternTime = 0.f;
				m_fAnimationStart = 0.f;
			}
		}
	}
}

void CSerpentTail::State_Change()
{
	switch (m_eSerpentPattern)
	{
	case SERPENT::TAILSHOTGUN:
		m_bTriple = true;
	case SERPENT::TAILGUN:
		m_bAttack = true;
		m_fAnimationSpeed = 0.5f;
		m_fPatternSpeed = 10.f;
		break;
	case SERPENT::RAGER:
		m_bAttack = false;
		break;
	}
}

void CSerpentTail::Select_Armor()
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

