#include "stdafx.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Collider.h"
#include "Jet_Enemy.h"
#include "Player.h"
CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}


//bool CCollision_Manager::Check_Sphere(CGameObject * _Dst, CGameObject * _Src)
//{
//	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
//	float fDis = static_cast<CCollider*>(_Src)->Get_ColSphereSize() + static_cast<CCollider*>(_Src)->Get_ColSphereSize();
//	float fDia = D3DXVec3Length(&vPos);
//	if (fDia <= fDis)
//	{
//		return true;
//	}
//	return false;
//}

bool CCollision_Manager::Check_Sphere(CCollider * _Dst, CCollider * _Src)
{
	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
	float fDis = _Src->Get_ColSphereSize() + _Src->Get_ColSphereSize();
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

// 적,플레이어 서치
void CCollision_Manager::Collision_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Rect_Sphere(pSrc, pDst))
			{
				//UI선택
				LONG RectSize = pSrc->Get_ColRect().right - pSrc->Get_ColRect().left;

				//float fLength = D3DXVec3Length(&(pDst->Get_Target()->Get_ObjInfo().vPos - pSrc->Get_ObjInfo().vPos));
				//플레이어와 몬스터의 차이
				if (RectSize == WINCX)
				{
					static_cast<CJet_Enemy*>(pDst->Get_Target())->Set_UiState(ARROW::END);
					static_cast<CPlayer*>(pSrc->Get_Target())->Set_Zoom(true);
					break;
				}
				else if (RectSize == WINCX*2)
				{
					static_cast<CJet_Enemy*>(pDst->Get_Target())->Set_UiState(ARROW::DISTANS_LEVEL2);
					break;
				}
				else if (RectSize == WINCX*3)
				{
					static_cast<CJet_Enemy*>(pDst->Get_Target())->Set_UiState(ARROW::DISTANS_LEVEL1);
					break;
				}
				else if (RectSize == WINCX*4)
				{
					static_cast<CJet_Enemy*>(pDst->Get_Target())->Set_UiState(ARROW::DISTANS_LEVEL0);
					break;
				}
				else
				{
					static_cast<CJet_Enemy*>(pDst->Get_Target())->Set_UiState(ARROW::DISTANS_FAR);
					break;
				}
			}
		}
	}
}
//렉트랑 구 충돌
bool CCollision_Manager::Check_Rect_Sphere(CCollider* _Dst, CCollider* _Src)
{

	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
	if (vPos.x < 0)
		vPos.x *= -1.f;
	if (vPos.y < 0)
		vPos.y *= -1.f;

	//float fY = abs((_Dst->Get_ColRect().bottom - _Dst->Get_ColRect().top) - _Src->Get_ColSphereSize());

	float fDisX = ((float)abs(_Dst->Get_ColRect().right - _Dst->Get_ColRect().left) + _Src->Get_ColSphereSize())*0.5f;
	float fDisY = ((float)abs(_Dst->Get_ColRect().bottom - _Dst->Get_ColRect().top) + _Src->Get_ColSphereSize())*0.5f;

	if (vPos.x < fDisX && vPos.y < fDisY)
	{
		return true;
	}
	return false;
}



//플레이어서치 적비행기 
//비행기 UI 상태 변경 
//void CCollision_Manager::Collision_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src)
//{
//	for (auto& pDst : _Dst)
//	{
//		for (auto& pSrc : _Src)
//		{
//			if (Check_Sphere(pDst, pSrc))
//			{
//				if (SearchSizeFar == pDst->Get_ColSphereSize())
//					static_cast<CJet_Enemy*>(pSrc->Get_Target())->Set_JetState(ENEMY::DISTANS_FAR);
//				if (SearchSizeLv0 == pDst->Get_ColSphereSize())
//					static_cast<CJet_Enemy*>(pSrc->Get_Target())->Set_JetState(ENEMY::DISTANS_LEVEL0);
//				if (SearchSizeLv1 == pDst->Get_ColSphereSize())
//					static_cast<CJet_Enemy*>(pSrc->Get_Target())->Set_JetState(ENEMY::DISTANS_LEVEL1);
//				if (SearchSizeLv2 == pDst->Get_ColSphereSize())
//					static_cast<CJet_Enemy*>(pSrc->Get_Target())->Set_JetState(ENEMY::DISTANS_LEVLE2);
//			}
//		}
//	}
//}