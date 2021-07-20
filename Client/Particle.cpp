#include "stdafx.h"
#include "Particle.h"
#include "Prefab_Manager.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
CParticle::CParticle() : m_fTime(0.f),m_fSpawnSpeed(0.f),m_fSpawnTime(0.f), m_fReduceSize(0.f), m_fMinSize(0.f)
{
}


CParticle::~CParticle()
{
}

CGameObject * CParticle::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, EFFECT::TYPE _eEffectType)
{
	CGameObject* pInstance = new CParticle;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	static_cast<CParticle*>(pInstance)->Set_EffectType(_eEffectType);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}


HRESULT CParticle::Ready_GameObject()
{
	m_tInfo.vSize = { 1.1f,1.1f,0.f };

	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()
		->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey
			+ m_pObjectInfo->wstrIdleImage_StateKey)->iMax_Index;

	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::EFFECT;
	InitParticle();


	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, 10.f, COLLIDER::EFFECT));

	return S_OK;
}
void CParticle::Move()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };

	m_fSpawnTime += fTime;
	if (m_fSpawnTime > m_fSpawnSpeed)
	{
		//m_fTime += fTime;
		_vec3 vNormalVel = m_vVelocity;
		D3DXVec3Normalize(&vNormalVel, &vNormalVel);

		m_tInfo.vPos += (m_tInfo.vDir*fTime*m_fAccel + (m_vVelocity*fTime)) + (_vec3{ 0.f,1.f,0.f }*0.5f*200.f *m_fSpawnTime*m_fTime);

		if (!m_pTexInfo)
			return;
		//��ƼŬ ���� ���� �� ����Ʈ���� ���� 

		CSpawn_Manager::Spawn(m_eEffectType, m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)) - (vNormalVel*0.2f), false, m_fSize);
		m_fSize -= m_fReduceSize;
		if (m_fSize < m_fMinSize)
			m_bDead = true;
		m_fSpawnTime = 0.f;

	}
}

void CParticle::InitParticle()
{

	switch (m_eEffectType)
	{
	case EFFECT::ROCKET_BOOM_PTFIRE:
		m_fAccel = 1200.f;
		m_fRegistPower = 2.f;
		m_tFrame.fFrameSpeed = 40.f;
		m_fSize = 0.5f;
		m_fSpawnSpeed = 0.01f;
		m_fReduceSize = 0.05f;
		m_fMinSize = 0.1f;
		break;
	//case EFFECT::FIRE_BOSS:
	//	m_fAccel = 500.f;
	//	m_fRegistPower = 2000.f;
	//	m_tFrame.fFrameSpeed = 40.f;
	//	m_fReduceSize = 0.05;
	//	m_fSize = 0.2f;
	//	m_fSpawnSpeed = 0.03f;
	//	m_fMinSize = 0.01f;
	//	break;
	}
}


int CParticle::Update_GameObject()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}
	Move();
	return OBJ_NOEVENT;
}

void CParticle::Late_Update_GameObject()
{
	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CParticle::Render_GameObject()
{
	WriteMatrix();
}

void CParticle::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fStartFrame = 0.f;
	}
}



void CParticle::Release_GameObject()
{
}

void CParticle::State_Change()
{
}
