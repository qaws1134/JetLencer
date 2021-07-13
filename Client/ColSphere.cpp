#include "stdafx.h"
#include "ColSphere.h"


CColSphere::CColSphere()
{
}


CColSphere::~CColSphere()
{
}


CCollider * CColSphere::Create(CGameObject* pTarget,COMBAT _tCombat,_vec3 vOffsetDir ,float fCollisionSize, COLLIDER::ID eColId)
{
	CCollider* pInstance = new CColSphere;
	static_cast<CCollider*>(pInstance)->Set_CollisionSize(fCollisionSize);
	static_cast<CCollider*>(pInstance)->Set_ColId(eColId);
	static_cast<CCollider*>(pInstance)->Set_OffSetDir(vOffsetDir);

	static_cast<CCollider*>(pInstance)->Set_CombatInfo(_tCombat);
	pInstance->Set_Target(pTarget);
	
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CCollider * CColSphere::Create(CGameObject * pTarget, COMBAT _tCombat, float fCollisionSize, COLLIDER::ID eColId)
{
	CCollider* pInstance = new CColSphere;
	static_cast<CCollider*>(pInstance)->Set_CollisionSize(fCollisionSize);
	static_cast<CCollider*>(pInstance)->Set_ColId(eColId);
	static_cast<CCollider*>(pInstance)->Set_CombatInfo(_tCombat);
	pInstance->Set_Target(pTarget);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CCollider * CColSphere::Create(CGameObject * pTarget, float fCollisionSize, COLLIDER::ID eColId)
{
	CCollider* pInstance = new CColSphere;
	static_cast<CCollider*>(pInstance)->Set_CollisionSize(fCollisionSize);
	static_cast<CCollider*>(pInstance)->Set_ColId(eColId);
	pInstance->Set_Target(pTarget);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CColSphere::WriteMatrix()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();

	D3DXVECTOR2 vLinePos[11];
	for (int i = 0; i< 10; i++)
	{
		vLinePos[i] = { cosf(D3DXToRadian(float(i * 36))),-sinf(D3DXToRadian(float(i * 36)))};

		vLinePos[i] *= m_fSphereSize;
		vLinePos[i] += D3DXVECTOR2{ m_tInfo.vPos.x + vScroll.x ,m_tInfo.vPos.y + vScroll.y};
	}
	vLinePos[10] = { cosf(D3DXToRadian(float(0 * 36))),-sinf(D3DXToRadian(float(0 * 36))) };
	vLinePos[10] *= m_fSphereSize;
	vLinePos[10] += D3DXVECTOR2{ m_tInfo.vPos.x + vScroll.x   ,m_tInfo.vPos.y + vScroll.y };

	CGraphic_Device::Get_Instance()->Get_Line()->SetWidth(3.0f);
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePos, 11, D3DCOLOR_ARGB(255, 255, 255, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}
