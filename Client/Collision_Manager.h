#pragma once
class CGameObject;
class CCollision_Manager
{
public:
	CCollision_Manager();
	~CCollision_Manager();

public:
	//static bool Check_Sphere(CGameObject* _Dst, CGameObject* _Src);
	static bool Check_Sphere(CCollider* _Dst, CCollider* _Src);
	static bool Check_Sphere(CCollider* _Dst, CCollider* _Src,float * _fDis);
	static bool Check_Rect_Sphere(CCollider* _Dst, CCollider* _Src);


	static void Collision_Player_Enemy(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Player_Enemy_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Enemy_Player_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_PlayerBeam(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_EnemyBeam(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Enemy_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Rocket_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_PlayerBullet_Rocket(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_PlayerBeam_Rocket(list<CCollider*>& _Dst, list<CCollider*>& _Src);

	static void Collision_Boss_Player_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_GunDrone_Enemy(list<CCollider*>& _Dst, list<CCollider*>& _Src);


};

