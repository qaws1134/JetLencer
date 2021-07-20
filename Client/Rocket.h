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

	// CBullet을(를) 통해 상속됨
	virtual void Late_Update_GameObject() override;
	void Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity);



	CGameObject* m_pTargetUi;
	float m_fTargetTime;
	float m_fTargetSpeed;

};
#endif // !__ROCKET_H__
