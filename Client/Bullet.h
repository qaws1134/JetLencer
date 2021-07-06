#pragma once
#include "GameObject.h"
class CBullet abstract :public CGameObject 
{

public:
	explicit CBullet();
	virtual ~CBullet();
public:
	//static	CGameObject* Create(const OBJECTINFO*	_tObjectInfo, D3DXVECTOR3 _vPos, float _fAngle );

	virtual HRESULT Ready_GameObject() PURE;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	void Set_Type(BULLET::TYPE _eID) { m_eID = _eID; }
protected:
	virtual void State_Change() PURE;
	virtual void Move() PURE;
	virtual void DeadEffect()PURE;
	
protected:
	BULLET::TYPE m_eID;

};

