#pragma once
#ifndef __JET_H__
#define __JET_H__

#include "GameObject.h"
class CJet :public CGameObject
{
protected:
	explicit CJet();
public:
	virtual ~CJet();

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() PURE;
	virtual int Update_GameObject() PURE;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() PURE;



public :
	void			Set_Dmg(int _iAtk) { m_tCombatInfo.iHp += _iAtk; }

public :
	COMBAT			Get_CombatInfo() { return m_tCombatInfo; }


public :
	void			InitJet();
	void			JetAngleCheck();
	virtual void	TimeCheck();
	bool			AttackTime();

	void Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity);

	virtual void	DeadEffect()PURE;
	void			SpawnPtFire();


protected:
	float	m_fAttackTime;
	float	m_fAttackSpeed;

	float	m_fBoostAccel;
	float	m_fGravity;
	int		m_iMax_ptFireNum;
	float	m_fMaxSpeedOffSet;

	COMBAT m_tCombatInfo;

	bool	m_bAccel;


	BURNER::STATE		m_eBurnerState;
	BURNER::STATE		m_PreBurnerState;

	//const ANIMATION* ptFire;
	CGameObject* m_pBurner;
};


#endif // !__JET_H__
