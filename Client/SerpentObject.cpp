#include "stdafx.h"
#include "SerpentObject.h"
#include "Prefab_Manager.h"
#include "ColSphere.h"
#include "Spawn_Manager.h"
CSerpentObject::CSerpentObject() 
	: m_bBreak(false) 
	, m_bAnimation(false)
	, m_bAttack(false)
	, m_fAnimationStart(0.f)
	, m_fAnimationMax(0.f)
	, m_fAnimationSpeed(0.f)
	, m_bAttack_End(false)
	, m_fPatternSpeed(0.f)
	,m_fPatternTime(0.f)
	, m_fSpawnTime(0.f)
	, m_fSpawnSpeed(0.f)
	, m_bDeadStart(false)
	,m_fDeadEffectTime(0.f)
	,m_fDeadEffectSpeed(0.f)
{


}


CSerpentObject::~CSerpentObject()
{
}



int CSerpentObject::Update_GameObject()
{
	if (m_bDeadEffect)
		DeadEffect();
	if (m_bDead)
		return OBJ_DEAD;
	if (!m_pTexInfo)
		return OBJ_NOEVENT;
	if (!m_pTarget)
		return OBJ_NOEVENT;

	Ai_State();
	State_Change();
	JetAngleCheck();
	return OBJ_NOEVENT;
}

void CSerpentObject::DeadEffect()
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
			m_bDead = true;
			m_bDeadStart = false;
		}
	}
}

void CSerpentObject::InitJet()
{
	m_tInfo.vSize = { 1.2f,1.2f,0.f };
	m_vVelocity = { 1.f,1.f,0.f };

	m_eRenderId = RENDERID::ENENY;
	m_fAngleSpeed = 300.f;
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 8;
	m_tFrame.fStartFrame = 0;

	FAILED_CHECK_RETURN(Set_Texture(), );

	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = 22.5f;

	m_tCombatInfo.iHp = 15;
	iMaxHp = 15;
	m_tCombatInfo.iAtk = 1;

	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 25.f, COLLIDER::ENEMY));
	m_AniIdle = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey + m_pObjectInfo->wstrIdleImage_StateKey);
	m_AniCrash = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->wstrCrashImage_ObjectKey+ m_pObjectInfo->wstrCrashImage_StateKey);
	m_AniFrame = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->wstrDeathAnimImage_ObjectKey + m_pObjectInfo->wstrDeathAnimImage_StateKey);


}

void CSerpentObject::Animation_Change(const ANIMATION* _pAnimation)
{
	m_tFrame.wstrObjKey = _pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = _pAnimation->wstrStateKey;
}

void CSerpentObject::Move()
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fColorTime += fTime;
	if (m_fColorTime > m_fColorSpeed)
	{
		m_tColor = { 255,255,255,255 };
	}
	m_vTarget_Dir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;

	TargetAngle_Check();
	if (fTime != 0)
	{
		m_tInfo.vPos = m_pTarget->Get_ObjInfo().vPos - m_vTarget_Dir*40.f;
		if(m_eSerpentPattern == SERPENT::CIRCLE_BULLET)
			m_tInfo.vPos += m_tInfo.vDir*8.5f;
		else
			m_tInfo.vPos += m_tInfo.vDir*3.f;
	}
}




