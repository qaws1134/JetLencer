#include "stdafx.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Collider.h"
#include "Jet_Enemy.h"
#include "Player.h"
#include "Mouse.h"
#include "Rocket.h"
#include "SoundMgr.h"
#include "Gundrone.h"
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



void CCollision_Manager::Collision_Player_Enemy_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (!static_cast<CPlayer*>(pDst->Get_Target())->SuperTime())
				{
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::STOP);

					pSrc->Get_Target()->Set_Color(MATCOLOR{ 200,0,255,255 });
					pSrc->Get_Target()->Set_ColorTime();
					pSrc->Set_Dmg(-pDst->Get_CombatInfo().iAtk);
					if (pSrc->Get_CombatInfo().iHp <= 0)
					{
						pSrc->Get_Target()->Set_DeadEffect(true);
						pSrc->Set_Dead(true);
					}
				}
				else
				{
					//잔상 온 
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::EVEDE);
				}
			}
		}
	}
}


void CCollision_Manager::Collision_Player_Enemy(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (!static_cast<CPlayer*>(pDst->Get_Target())->SuperTime())
				{
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::STOP);
					pSrc->Get_Target()->Set_DeadEffect(true);
					pSrc->Set_Dead(true);
				}
				else
				{
					//잔상 온 
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::EVEDE);
				}
			}
		}
	}
}
void CCollision_Manager::Collision_Boss_Player_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				static_cast<CMouse*>(CGameObject_Manager::Get_Instance()->Get_Mouse())->Set_MouseState(MOUSE::HIT);
				static_cast<CMouse*>(CGameObject_Manager::Get_Instance()->Get_Mouse())->Set_HitTime();

				pDst->Set_Dmg(-pSrc->Get_CombatInfo().iAtk);
				pDst->Get_Target()->Set_Color(MATCOLOR{ 200,0,255,255 });
				pDst->Get_Target()->Set_ColorTime();
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



//오브젝트, 총알
void CCollision_Manager::Collision_Enemy_Player_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				static_cast<CMouse*>(CGameObject_Manager::Get_Instance()->Get_Mouse())->Set_MouseState(MOUSE::HIT);
				static_cast<CMouse*>(CGameObject_Manager::Get_Instance()->Get_Mouse())->Set_HitTime();

				pDst->Set_Dmg(-pSrc->Get_CombatInfo().iAtk);
				pDst->Get_Target()->Set_Color(MATCOLOR{ 200,0,255,255 });
				pDst->Get_Target()->Set_ColorTime();
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
void CCollision_Manager::Collision_PlayerBeam(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (!pSrc->Get_Dead())
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
}


void CCollision_Manager::Collision_EnemyBeam(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (!static_cast<CPlayer*>(pDst->Get_Target())->SuperTime())
				{
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::STOP);
					pSrc->Set_Dead(true);
				}
				else
				{
					//잔상 온 
					static_cast<CPlayer*>(pDst->Get_Target())->Set_State(PLAYER::EVEDE);
				}
			}
		}
	}
}


// 적,플레이어 서치
void CCollision_Manager::Collision_Enemy_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Rect_Sphere(pSrc, pDst))
			{
				//UI선택
				LONG RectSize = pSrc->Get_ColRect().right - pSrc->Get_ColRect().left;
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
// 로켓,플레이어 서치
void CCollision_Manager::Collision_Rocket_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Rect_Sphere(pSrc, pDst))
			{
				if (pDst->Get_Target()->Get_Prefab()->eBulletType == BULLET::GUIDE)
				{
					LONG RectSize = pSrc->Get_ColRect().right - pSrc->Get_ColRect().left;
					//플레이어와 몬스터의 차이
					if (RectSize == WINCX)
					{
						static_cast<CRocket*>(pDst->Get_Target())->Set_UiState(ARROW::END);
						static_cast<CRocket*>(pDst->Get_Target())->Set_Sound(true);

						break;
					}
					else
					{
						static_cast<CRocket*>(pDst->Get_Target())->Set_UiState(ARROW::ROCKET_MARKER);
						static_cast<CRocket*>(pDst->Get_Target())->Set_Sound(false);
						break;
					}
				}
			}
		}
	}
}
// 로켓,플레이어 총알
void CCollision_Manager::Collision_PlayerBullet_Rocket(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (pDst->Get_Target()->Get_Prefab()->eBulletType == BULLET::GUIDE)
				{
					if (!pSrc->Get_Dead())
					{
						pDst->Set_Dmg(-pSrc->Get_CombatInfo().iAtk);
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
	}
}
// 로켓,플레이어 총알
void CCollision_Manager::Collision_PlayerBeam_Rocket(list<CCollider*>& _Dst, list<CCollider*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				if (pDst->Get_Target()->Get_Prefab()->eBulletType == BULLET::GUIDE)
				{
					if (!pSrc->Get_Dead())
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
	}
}
//적 , 드론 서치
void CCollision_Manager::Collision_GunDrone_Enemy(list<CCollider*>& _Src, list<CCollider*>& _Dst)
{

	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pSrc, pDst))
			{
				static_cast<CGundrone*>(pDst->Get_Target())->Set_Attack(true);
				break;
			}
			else
			{
				static_cast<CGundrone*>(pDst->Get_Target())->Set_Attack(false);
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


bool CCollision_Manager::Check_Sphere(CCollider * _Dst, CCollider * _Src)
{
	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
	float fDis = _Dst->Get_ColSphereSize() + _Src->Get_ColSphereSize();
	float fDia = D3DXVec3Length(&vPos);
	if (fDia <= fDis)
	{
		return true;
	}
	return false;
}

bool CCollision_Manager::Check_Sphere(CCollider * _Dst, CCollider * _Src,float *_fDis)
{
	_vec3 vPos = _Dst->Get_ObjInfo().vPos - _Src->Get_ObjInfo().vPos;
	float fDis = _Dst->Get_ColSphereSize() + _Src->Get_ColSphereSize();
	float fDia = D3DXVec3Length(&vPos);
	if (fDia <= fDis)
	{
		_fDis = &fDia;
		return true;
	}
	return false;
}