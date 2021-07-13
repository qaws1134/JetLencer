#include "stdafx.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Collider.h"
CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}


void searchrect()
{

}


bool CCollision_Manager::Check_Sphere(CGameObject * _Dst, CGameObject * _Src)
{
	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
	float fDis = static_cast<CCollider*>(_Src)->Get_ColSphereSize() + static_cast<CCollider*>(_Src)->Get_ColSphereSize();
	float fDia = D3DXVec3Length(&vPos);
	if (fDia <= fDis)
	{
		return true;
	}
	return false;
}

//오브젝트, 총알
void CCollision_Manager::Collision_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				pDst->Set_Dmg(-pSrc->Get_CombatInfo().iAtk);
				pDst->Get_Target()->Set_Color(MATCOLOR{ 255,255,0,0 });
				if (pDst->Get_CombatInfo().iHp <= 0)
				{
					pDst->Get_Target()->Set_DeadEffect(true);
					pDst->Set_Dead(true);
				}
				pSrc->Get_Target()->Set_DeadEffect(true);
				pSrc->Set_Dead(true);
			}
		}
	}
}

//오브젝트 , 빔
void CCollision_Manager::Collision_Beam(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				pDst->Set_Dmg(-pSrc->Get_CombatInfo().iAtk);
				if (pDst->Get_CombatInfo().iHp <= 0)
				{
					pDst->Get_Target()->Set_DeadEffect(true);
					pDst->Set_Dead(true);
				}
				pSrc->Set_Dead(true);
			}
		}
	}
}
