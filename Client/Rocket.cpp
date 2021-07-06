#include "stdafx.h"
#include "Rocket.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Spawn_Manager.h"
CRocket::CRocket()
{
}


CRocket::~CRocket()
{
}


CGameObject * CRocket::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed)
{
	CGameObject* pInstance = new CRocket;
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



HRESULT CRocket::Ready_GameObject()
{
	m_tInfo.vSize = { 1.2f,1.1f,0.f };
	m_vVelocity = { 0.f,0.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fAccel = 1500.f;
	return S_OK;
}

void CRocket::State_Change()
{

}

void CRocket::Move()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	
	m_vVelocity += m_tInfo.vDir*m_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&m_vVelocity);

	if (fVelLength > 1000.f)
	{
		m_vVelocity *= 1000.f / fVelLength;
	}

	m_tInfo.vPos += m_vVelocity*fTime; ;

	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	for (int i = 0; i < 10; i++)
	{
		if (!m_pTexInfo)
			return;
		CSpawn_Manager::Spawn(L"EffectPtfire", m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.5f) - (vNormalVel*((float)i)*1.5f), false,EFFECT::ROCKET_PTFIRE);
	}
}

void CRocket::DeadEffect()
{
	//오브젝트와 충돌이 되면 충돌 시 이펙트 생성
}
