#include "stdafx.h"
#include "SerpentBody.h"
#include "Spawn_Manager.h"
#include "Prefab_Manager.h"
#include "Collider.h"
CSerpentBody::CSerpentBody():m_bAniEnd(false)
{
	m_vecBody_Gun.reserve(8);
}


CSerpentBody::~CSerpentBody()
{
}

CGameObject* CSerpentBody::Create(const OBJECTINFO * _tObjectInfo)
{
	CGameObject* pInstance = new CSerpentBody;
	pInstance->Set_Prefab(_tObjectInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}



HRESULT CSerpentBody::Ready_GameObject()
{
	InitJet();

	m_fAnimationStart= 0.f;
	m_fAnimationMax = 6.f;
	m_fAnimationSpeed = 2.f;
	m_fSpawnSpeed = 0.1f;
	m_fSpawnTime = 0.f;
	for (int i = 0; i < 6; i++)
	{
		const ANIMATION* pAni = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EnemySerpent_Body_Gun"+ to_wstring(i+1));
		m_vecBody_Gun.emplace_back(pAni);
	}
	return S_OK;
}

void CSerpentBody::Ai_State()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	Select_Armor();

	//애니메이션 시작
	if (m_bAnimation)
	{
		m_fAnimationStart += m_fAnimationSpeed*fTime;
		if (m_fAnimationStart >= m_fAnimationMax)
		{
			m_fAnimationStart = m_fAnimationMax - 1;
			Animation_Change(m_vecBody_Gun[(int)m_fAnimationStart]);
			m_bAnimation = false;
			m_bAttack = true;
		}
		else
		{

			if ((int)m_fAnimationStart < 6)
				Animation_Change(m_vecBody_Gun[(int)m_fAnimationStart]);
		}
	}

	//공격 시작
	if (m_bAttack)
	{
		_vec3 vCross = {};
		D3DXVec3Cross(&vCross, &m_vCross, &m_tInfo.vDir);
		CSpawn_Manager::Spawn(L"Serpent_Bullet", m_tInfo.vPos, m_fAngle, vCross*0.1f);
		m_bAttack = false;
		m_bAniEnd = true;
		m_bAttack_End = true;
	}

	if (m_bAniEnd)
	{
		if (!m_bBreak)
		{
			m_fAnimationStart -= m_fAnimationSpeed*fTime;
			if (m_fAnimationStart <= 0)
			{
				m_fAnimationStart = 0;
				Animation_Change(m_vecBody_Gun[(int)m_fAnimationStart]);
			}
		}
	}
	
	
	Move();
	
}

void CSerpentBody::State_Change()
{
	if (!m_bAttack_End)
	{
		switch (m_eSerpentPattern)
		{
		case SERPENT::CIRCLE_BULLET:
			m_bAnimation = true;
			m_bAniEnd = false;
			m_bAttack = false;
			break;
		}
	}
}

void CSerpentBody::DeadEffect()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bDeadStart)
	{
		m_fDeadEffectTime += fTime;
		if (m_fDeadEffectTime > m_fDeadEffectSpeed)
		{
			m_fDeadEffectTime = 0.f;
			CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
			RandomEffect(EFFECT::EXPLOSION1, 3, 60);
			RandomEffect(EFFECT::EXPLOSION2, 3, 60);
			RandomEffect(EFFECT::EXPLOSION3, 3, 60);
			for (auto& iter : m_vecBody_Gun)
			{
				iter = nullptr;
			}
			m_vecBody_Gun.clear();

			m_bDead = true;
			m_bDeadStart = false;
		}

	}
	else 
	{
		m_fSpawnTime += fTime;
		if (m_fSpawnTime > m_fSpawnSpeed)
		{
			m_fSpawnTime = 0.f;
			RandomEffect(EFFECT::FIRE_BOSS, 3, 20);
		}
	}
}

void CSerpentBody::Select_Armor()
{
	if (10 < m_vecCollider.front()->Get_CombatInfo().iHp)
	{
		Animation_Change(m_AniIdle);
	}
	else if (5 < m_vecCollider.front()->Get_CombatInfo().iHp)
	{
		m_bBreak = true;
		Animation_Change(m_AniFrame);
	}
	else if (0< m_vecCollider.front()->Get_CombatInfo().iHp)
	{
		Animation_Change(m_AniCrash);
	}
	else
	{
		Animation_Change(m_AniCrash);
		m_bCrash = true;
	}

	
}


//바디 공격 시 gun1~gun6까지  멀티텍스쳐 변환 