#pragma once
#ifndef __NORMAL_H__
#define __NORMAL_H__
#include "Bullet.h"

class CNormal :
	public CBullet
{
private:
	explicit CNormal();
public :
	virtual ~CNormal();

public :
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed);

	virtual HRESULT Ready_GameObject() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
	virtual void State_Change() override;
};
#endif // !__NORMAL_H__
