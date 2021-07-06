#pragma once
#ifndef __ROCKET_H__
#define __ROCKET_H__
#include "Bullet.h"

class CRocket  :
	public CBullet
{
private:
	explicit CRocket();
public:
	virtual ~CRocket();

public:
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed);

	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
};
#endif // !__ROCKET_H__
