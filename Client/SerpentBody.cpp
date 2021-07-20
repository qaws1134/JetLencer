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
	m_fAnimationSpeed = 5.f;

	for (int i = 0; i < 6; i++)
	{
		const ANIMATION* pAni = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EnemySerpent_Body_Gun"+ to_wstring(i+1));
		m_vecBody_Gun.emplace_back(pAni);
	}
	return S_OK;
}

void CSerpentBody::Ai_State()
{

	Select_Armor();
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	//�ִϸ��̼� ����
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
			if((int)m_fAnimationStart < 6)
				Animation_Change(m_vecBody_Gun[(int)m_fAnimationStart]);
		}
	}

	//���� ����
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
		m_fAnimationStart -= m_fAnimationSpeed*fTime;
		if (m_fAnimationStart < 0)
		{
			m_fAnimationStart = 0;
			Animation_Change(m_vecBody_Gun[(int)m_fAnimationStart]);
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


//�ٵ� ���� �� gun1~gun6����  ��Ƽ�ؽ��� ��ȯ 