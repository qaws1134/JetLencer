#pragma once
#include "Collider.h"
class CColRect :
	public CCollider
{
public:
	explicit CColRect();
	virtual ~CColRect();

	static CCollider * Create(CGameObject* pTarget, _vec3 LeftTop,_vec3 RightBottom, COLLIDER::ID _eColId);

	// CCollider을(를) 통해 상속됨
	virtual void WriteMatrix() override;
};

