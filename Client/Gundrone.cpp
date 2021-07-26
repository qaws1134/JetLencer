#include "stdafx.h"
#include "Gundrone.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
CGundrone::CGundrone() : m_fDeleteTime(0.f), m_bAttack(false), m_fMaxSpeedOffSet(0.f), m_bMove(false),m_fMoveTime(0.f)
{
}

CGameObject * CGundrone::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed)
{
	CGameObject* pInstance = new CGundrone;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	static_cast<CBullet*>(pInstance)->Set_Type((BULLET::TYPE)_tObjectInfo->eBulletType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CGundrone::~CGundrone()
{
}

HRESULT CGundrone::Ready_GameObject()
{
	m_tInfo.vSize = { 1.1f,1.1f,0.f };

	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()
		->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey
			+ m_pObjectInfo->wstrIdleImage_StateKey)->iMax_Index;

	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::OBJECT;
	m_tFrame.fFrameSpeed = 30.f;
	m_fMaxSpeedOffSet = 30.f;

	m_vVelocity = { 1.f,1.f,0.f };
	m_vGravity = { 0.f,1.f,0.f };
	m_fRegistPower = 4000.f;
	m_fAngleSpeed = 300.f;
	m_fAccel = 500.f;
	m_fMaxSpeed = 500.f;


	m_tCombatInfo.iHp = 5;
	m_tCombatInfo.iAtk = 1;
	m_fAttTime = 0.f;


	//1. 서치 컬라이더
	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 500.f, COLLIDER::GUNDRONE));
	//m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 30.f, COLLIDER::EFFECT));

	return S_OK;
}

void CGundrone::State_Change()
{

	//delete time 
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	/*m_fDeleteTime +=fTime;
	if (m_fDeleteTime > 10.f)
		m_bDeadEffect = true;*/

	if (!m_bMove)
	{
		m_fMoveTime += fTime;
		if (m_fMoveTime > 1.f)
			m_bMove = true;
	}
}

void CGundrone::Move()
{
	//DIR로 일정 위치 이동 후 
	//제자리에서 미사일 발사 

	if (!m_bMove)
	{
		m_vTarget_Dir = { cosf(D3DXToRadian(m_fAngle)),-sinf(D3DXToRadian(m_fAngle)),0.f };
		TargetAngle_Check();
		Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	}
	else
	{
		m_pTarget = CGameObject_Manager::Get_Instance()->Get_Target(this, OBJID::ENEMY);
		if (m_bAttack)
		{
			if (m_pTarget)
			{
				float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
				m_vTarget_Dir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
				TargetAngle_Check();
				m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)),-sinf(D3DXToRadian(m_fAngle)),0.f };
				m_fAttTime += fTime;
				if (m_fAttTime > 0.2f)
				{
					m_fAttTime = 0.f;
					CSpawn_Manager::Spawn(L"Minigun", m_tInfo.vPos, m_fAngle, m_tInfo.vDir, true);
				}
			}
		}
	}
}

void CGundrone::DeadEffect()
{
	CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
	RandomEffect(EFFECT::EXPLOSION1, 3, 60);
	RandomEffect(EFFECT::EXPLOSION2, 3, 60);
	RandomEffect(EFFECT::EXPLOSION3, 3, 60);
	m_bDead = true;
}



void CGundrone::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fStartFrame = 0.f;
	}
}

void CGundrone::Late_Update_GameObject()
{
	State_Change();
	Frame_Change();
}

void CGundrone::Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity)
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