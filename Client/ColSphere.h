#pragma once
#include "Collider.h"
class CColSphere :
	public CCollider
{
private:
	explicit CColSphere();
public:
	virtual ~CColSphere();
	static CCollider * Create(CGameObject* pTarget, COMBAT _tCombat, _vec3 vOffsetDir,float fCollisionSize,COLLIDER::ID eColId);
	static CCollider * Create(CGameObject* pTarget, COMBAT _tCombat, float fCollisionSize, COLLIDER::ID eColId);
	static CCollider * Create(CGameObject* pTarget, float fCollisionSize, COLLIDER::ID eColId);

	virtual void WriteMatrix() override;

};

