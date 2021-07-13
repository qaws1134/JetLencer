#include "stdafx.h"
#include "Beam.h"
#include "Prefab_Manager.h"
#include "GameObject_Manager.h"
#include "ColSphere.h"
CBeam::CBeam() 
{
}


CBeam::~CBeam()
{
}

CGameObject * CBeam::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, BULLET::SUBWEAPON _eSubId)
{
	CGameObject* pInstance = new CBeam;
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


//void CBeam::WriteMatrix()
//{
//	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
//
//	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
//	D3DXMatrixIdentity(&matScale);
//	D3DXMatrixIdentity(&matTrans);
//	D3DXMatrixIdentity(&matRotZ);
//
//	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
//	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
//	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);
//
//	matWorld = matScale *matRotZ* matTrans;
//	float fCenterX = 0;
//	float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
//	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
//	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
//}

HRESULT CBeam::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_vVelocity = { 0.f,0.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 3;
	m_tFrame.fStartFrame = 0;
	m_eRenderId = RENDERID::BULLET;
	m_bCenter = true;
	InitState();
	return S_OK;
}

void CBeam::State_Change()
{
	m_fCenterX = 0.f;
	m_fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
	
	m_tInfo.vSize -= _vec3{ 0.f,0.02f,0.f };
	if (m_tInfo.vSize.y < 0)
	{
		m_bDeadEffect = true;
	}
}

void CBeam::Move()
{
	m_fAngle = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Angle();
	m_tInfo.vPos = CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vPos;
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };

	int i = 0;
	for (auto& VecCol : m_vecCollider)
	{	
		i++;
		VecCol->Set_OffSetDir(m_tInfo.vDir*(float)(i)*40.f);
	}
}

void CBeam::DeadEffect()
{
	for (auto& iter : m_vecCollider)
	{
		iter->Set_Dead(true);
	}
	m_vecCollider.clear();
	m_vecCollider.shrink_to_fit();
	m_bDead = true;
}

void CBeam::InitState()
{
	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)) ,-sinf(D3DXToRadian(m_fAngle)) ,0.f };
		m_vecCollider.reserve(32);
	switch (m_eSubWeaponState)
	{
	case BULLET::CHARGE_1:
		m_tFrame.fStartFrame = 0;
		m_tCombatInfo.iAtk = 5;
		for (int i = 0; i < 20; i++)
			m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i+1)*40.f, 10.f, COLLIDER::PLAYER_BULLET_BEAM));
		break;
	case BULLET::CHARGE_2:
		m_tFrame.fStartFrame = 1;
		m_tCombatInfo.iAtk = 8;
		for (int i = 0; i < 20; i++)
			m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*40.f, 15.f, COLLIDER::PLAYER_BULLET_BEAM));
		m_tInfo.vSize = { 1.f,1.3f,0.f };
		break;
	case BULLET::CHARGE_3:
		m_tFrame.fStartFrame = 2;
		m_tCombatInfo.iAtk = 10;
		for (int i = 0; i < 20; i++)
			m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, m_tInfo.vDir*(float)(i + 1)*40.f, 20.f, COLLIDER::PLAYER_BULLET_BEAM));
		m_tInfo.vSize = { 1.f,1.8f,0.f };
		break;
	}
}
