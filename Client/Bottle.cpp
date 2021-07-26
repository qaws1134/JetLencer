#include "stdafx.h"
#include "Bottle.h"
#include "Spawn_Manager.h"
#include "Prefab_Manager.h"
#include "ColSphere.h"
#include "SoundMgr.h"
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
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()
		->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey 
			+ m_pObjectInfo->wstrIdleImage_StateKey)->iMax_Index;

	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fAccel = 700.f;
	m_fRegistPower = 2.f;
	m_tFrame.fFrameSpeed = 30.f;
	m_tCombatInfo.iAtk = 5;
	m_tCombatInfo.iHp = 1;

	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 35.f, COLLIDER::PLAYER_BULLET));
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_BOTTLE);
	CSoundMgr::Get_Instance()->PlaySound(L"Player_Bottle.mp3", CSoundMgr::PLAYER_BOTTLE);


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

	for (int i = 0; i < 15; i++)
	{
		if (m_bDeadEffect)
			return;
		if (!m_pTexInfo)
			return;
		CSpawn_Manager::Spawn(EFFECT::ROCKET_PTFIRE, m_tInfo.vPos - m_tInfo.vDir*16.f*0.8f + (vNormalVel)*float(i)*0.2f);
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

void CBottle::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x > Map_Width || m_tInfo.vPos.x < 0
		|| m_tInfo.vPos.y < 0)
	{
		m_bDead = true;
	}
	if (m_tInfo.vPos.y > Map_Height + 70)
		m_bDeadEffect = true;
	State_Change();
}


void CBottle::DeadEffect()
{
	if (m_tInfo.vPos.y > Map_Height + 70)
	{
		CSpawn_Manager::Spawn(EFFECT::GROUND_WATERSPLASH_FAST, _vec3{ m_tInfo.vPos.x , float(Map_Height + 72) ,0.f }, false, 1.0f);
	}
	else
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT_BOOM1);
		CSoundMgr::Get_Instance()->PlaySound(L"Explosion1.mp3", CSoundMgr::EFFECT_BOOM1);
		CSpawn_Manager::Spawn(EFFECT::OBJECT_IMPACT, m_tInfo.vPos, false);
		CSpawn_Manager::Spawn(EFFECT::MEGA_EXPLOSION1, m_tInfo.vPos, false);
		RandomEffect(EFFECT::EXPLOSION1, 3, 60);
		RandomEffect(EFFECT::EXPLOSION2, 3, 60);
		RandomEffect(EFFECT::EXPLOSION3, 3, 60);
	}
	m_bDead = true;
}
