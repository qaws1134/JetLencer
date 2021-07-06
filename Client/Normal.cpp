#include "stdafx.h"
#include "Normal.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
CNormal::CNormal()
{
}


CNormal::~CNormal()
{
}


CGameObject * CNormal::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle,_vec3 _vSpeed)
{
	CGameObject* pInstance = new CNormal;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	pInstance->Set_Angle(_fAngle);
	pInstance->Set_Speed(_vSpeed);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}



HRESULT CNormal::Ready_GameObject()
{
	m_tInfo.vSize = { 0.28f,0.28f,0.f };
	m_tInfo.vDir.z = 0.f;
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_tFrame.wstrObjKey + m_tFrame.wstrStateKey)->iMax_Index;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_fSpeed = 3.f;
	
	return S_OK;
}

void CNormal::State_Change()
{
	if (m_pObjectInfo->eObjId == OBJID::PLAYER_BULLET)
	{
		if (m_tInfo.vSize.x < 1.2f)
			m_tInfo.vSize += _vec3{ 0.085f,0.085f,0.f };
		else
			m_tFrame.fStartFrame = 1;
	}
}

void CNormal::Move()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
	float fVelocity = D3DXVec3Length(&m_vVelocity);

	m_tInfo.vDir *= 850.f*fTime;
	m_tInfo.vDir+= CGameObject_Manager::Get_Instance()->Get_Player()->Get_Velocity()*fTime;
	m_tInfo.vPos += m_tInfo.vDir;

}

void CNormal::DeadEffect()
{
	//오브젝트와 충돌이 되면 충돌 시 이펙트 생성
}
