#pragma once
#include "GameObject.h"
class CPlayer final :public CGameObject
{
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT		Ready_GameObject() override;
	virtual int			Update_GameObject() override;
	virtual void		Late_Update_GameObject() override;
	virtual void		Render_GameObject() override;
	virtual void		Release_GameObject() override;
public:
	static CGameObject*	Create();

public :
	virtual void		State_Change();
	void				Frame_Change();
	void				PlayerAngle_Check();
	void				Key_State();
 	void				Keybord_OffSet();
	bool				AttackTime();
	bool				RocketTime();
	bool				ChargeShotTime();
	void				TimeCheck();
	void				Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity);
	void				DeadEffect();
private:
	const ANIMATION*	m_pAnimationInfo;
	float				m_fAttackTime;
	float				m_fAttackSpeed;
	float				m_fRocketTime[4];
	float				m_fRocketSpeed;
	bool				m_bRoll;
	float				m_fRollTime;
	float				m_fRollCoolSpeed;
	bool				m_bMega;
	float				m_fOffsetAngle;
	bool				m_bAccel;


	PLAYER::STATE		m_eState;
	PLAYER::STATE		m_ePreState;
	CGameObject*		m_pBurner;

};
