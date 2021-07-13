#pragma once
#include "Bullet.h"
class CGundrone :
	public CBullet
{
private:
	explicit CGundrone();
public:
	virtual ~CGundrone();

	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, BULLET::SUBWEAPON _eSubId);

	virtual HRESULT Ready_GameObject() override;

	virtual void State_Change() override;

	virtual void Move() override;

	virtual void DeadEffect() override;

	void Frame_Change();


	float m_fDeleteTime;
	bool	m_bAttack;

};

