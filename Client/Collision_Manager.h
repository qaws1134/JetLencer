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
	static bool Check_Rect_Sphere(CCollider* _Dst, CCollider* _Src);

	static void Collision_Bullet(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Beam(list<CCollider*>& _Dst, list<CCollider*>& _Src);
	static void Collision_Search(list<CCollider*>& _Dst, list<CCollider*>& _Src);
};

