#pragma once
#include "Bullet.h"
class CBeam :
	public CBullet
{
private:
	explicit CBeam();
public :
	virtual ~CBeam();

	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed,BULLET::SUBWEAPON _eSubId);

	// CBullet을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
	//void WriteMatrix();
	void InitState();
};

