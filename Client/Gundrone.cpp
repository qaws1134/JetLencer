#include "stdafx.h"
#include "Gundrone.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"
CGundrone::CGundrone() : m_fDeleteTime(0.f), m_bAttack(false)
{
}

CGameObject * CGundrone::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, BULLET::SUBWEAPON _eSubId)
{
	CGameObject* pInstance = new CGundrone;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	static_cast<CBullet*>(pInstance)->Set_SubWeaponState(_eSubId);
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


	//1. 서치 컬라이더
	//2. 몸체 컬라이더



	return S_OK;
}

void CGundrone::State_Change()
{
	if (m_bAttack)
	{
		//총알 소환 
		//주변 범위 내 몬스터 추적해서 해당 방향으로 +=10도 각도
		//CSpawn_Manager::Spawn(L"Normal",m_tInfo.vPos,)
	}


	//delete time 
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fDeleteTime +=fTime;
	if (m_fDeleteTime > 15.f)
		m_bDead = true;

}

void CGundrone::Move()
{
	//DIR로 일정 위치 이동 후 
	//제자리에서 미사일 발사 
	if (m_bAttack)
		return;
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_vVelocity += m_tInfo.vDir*m_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&m_vVelocity);
	if (fVelLength > 1200.f)
	{
		m_vVelocity *= 1200.f / fVelLength;
		m_bAttack = true;
	}
	m_tInfo.vPos += m_vVelocity*fTime;
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	


}

void CGundrone::DeadEffect()
{
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