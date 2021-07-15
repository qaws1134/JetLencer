#include "stdafx.h"
#include "Jet.h"
#include "GameObject_Manager.h"
#include "Prefab_Manager.h"
#include "Burner.h"
#include "Spawn_Manager.h"


CJet::CJet()
	: m_fAttackTime(0.f)
	, m_fAttackSpeed(0.f)
	, m_pBurner(nullptr)
	, m_fMaxSpeedOffSet(0.f)
	, m_fBoostAccel(0.f)
	, m_fGravity(0.f)
	, m_iMax_ptFireNum(0.f)
	, m_bAccel(false)
	, m_eBurnerState(BURNER::END)
	, m_PreBurnerState(BURNER::END)
{
	ZeroMemory(&m_tCombatInfo, sizeof(m_tCombatInfo));
}
CJet::~CJet()
{

}


void CJet::Late_Update_GameObject()
{

	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CJet::Render_GameObject()
{
	WriteMatrix();

}

void CJet::Release_GameObject()
{
}

void CJet::InitJet()
{
	m_vVelocity = { 1.f,1.f,0.f };
	m_tInfo.vSize = { 1.2f,1.2f,0.f };


	m_eRenderId = RENDERID::OBJECT;

	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_tFrame.wstrObjKey + m_tFrame.wstrStateKey)->iMax_Index;
	m_tFrame.fStartFrame = 0;
	m_fAngleSpeed = 180.f;

	m_fGravity = 200.f;
	m_iMax_ptFireNum = 10;
	m_fMaxSpeedOffSet = 30.f;

	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = -22.5f;

	m_pBurner = CBurner::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pBurner);
	
}

void CJet::JetAngleCheck()
{
	for (float i = 0; i < m_tFrame.fMaxFrame; i++)
	{
		if (m_fAngle <= m_fAngleOffset + (i*m_fAngle_per_Frame))
		{
			m_tFrame.fStartFrame = i;
			return;
		}
	}
}


void CJet::TimeCheck()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fAttackTime += fTime;
}

bool CJet::AttackTime()
{
   	if (m_fAttackTime > m_fAttackSpeed)
	{
		m_fAttackTime = 0.f;
		return true;
	}
	return false;
}

void CJet::Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity)
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vVec = m_vVelocity + vDir*_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&vVec);

	if (bGravity)
		m_vVelocity += vDir*_fAccel*fTime;

	if (fVelLength > _fMaxSpeed + m_fMaxSpeedOffSet)
	{
		float fVelLength = D3DXVec3Length(&m_vVelocity);
		float fRegist = m_fRegistPower * fTime;
		if (fRegist > fVelLength)
			fRegist = fVelLength;
		_vec3 vRegist = (m_vVelocity* -fRegist);
		if (bGravity)
			vRegist.y = 0;
		D3DXVec3Normalize(&vRegist, &vRegist);
		m_vVelocity += vRegist*10.f;
		//최대속도 
	}
	else if (fVelLength > _fMaxSpeed)
		m_vVelocity *= _fMaxSpeed / fVelLength;
	else
	{
		if (!bGravity)
			m_vVelocity += vDir*_fAccel*fTime;
	}

	m_tInfo.vPos += (m_vVelocity*fTime);
}



//스레드로 차차 생각해보기
void CJet::SpawnPtFire()
{
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	if (m_bAccel)
	{
		for (int i = 0; i < m_iMax_ptFireNum; i++)
			CSpawn_Manager::Spawn(EFFECT::JET_PTFIRE, m_tInfo.vPos
				- m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.8f) 
				- (vNormalVel*((float)i)), false);
	}

}
