#include "stdafx.h"
#include "Bottle.h"
#include "Spawn_Manager.h"
#include "Prefab_Manager.h"
#include "ColSphere.h"
CBottle::CBottle():m_fTime(0.f)
{
}


CBottle::~CBottle()
{
}

CGameObject * CBottle::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed)
{
	CGameObject* pInstance = new CBottle;
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

HRESULT CBottle::Ready_GameObject()
{
	m_tInfo.vSize = { 1.1f,1.1f,0.f };

	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = CPrefab_Manager::Get_Instance()
		->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey 
			+ m_pObjectInfo->wstrIdleImage_StateKey)->iMax_Index;

	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fAccel = 500.f;
	m_fRegistPower = 2.f;
	m_tFrame.fFrameSpeed = 30.f;
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 13.f, COLLIDER::PLAYER_BULLET));
	return S_OK;
}

void CBottle::State_Change()
{
	Frame_Change();
}

void CBottle::Move()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	m_fTime += fTime;
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);

	m_tInfo.vPos += (m_tInfo.vDir*fTime*m_fAccel +(m_vVelocity*fTime))+ (_vec3{0.f,1.f,0.f}*0.5f*20.f *m_fTime*m_fTime);

	for (int i = 0; i < 10; i++)
	{
		if (!m_pTexInfo)
			return;
		CSpawn_Manager::Spawn(EFFECT::ROCKET_PTFIRE,m_tInfo.vPos - _vec3(m_tInfo.vDir.x,0.f,0.f)*((float)(m_pTexInfo->tImageInfo.Width)) - (vNormalVel*0.2f), false);
	}
}


void CBottle::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fStartFrame = 0.f;
	}
}


void CBottle::DeadEffect()
{
	m_bDead = true;
}
