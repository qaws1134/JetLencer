#pragma once
#ifndef __BOTTLE_H__
#define __BOTTLE_H__
#include "Bullet.h"
class CBottle :public CBullet
{
private:
	explicit CBottle();
public :
	virtual ~CBottle();

public:
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed);
public :
	// CBullet을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
	//virtual void WriteMatrix();

	void Frame_Change();
	float m_fTime;
};
#endif // !__BOTTLE_H__
