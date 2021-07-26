#pragma once
#include "Bullet.h"
class CGundrone :
	public CBullet
{
private:
	explicit CGundrone();
public:
	virtual ~CGundrone();

	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed);
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;

	void Set_Attack(bool _bAttack) { m_bAttack = _bAttack; }
	

	void Frame_Change();
	void Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity);

private:

	float m_fDeleteTime;
	bool	m_bAttack;
	float m_fMaxSpeedOffSet;
	float m_fMoveTime;
	bool m_bMove;


	float m_fAttTime;
	// CBullet을(를) 통해 상속됨
	virtual void Late_Update_GameObject() override;

};

