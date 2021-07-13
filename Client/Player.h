#pragma once
#include "Jet.h"
class CPlayer :public CJet
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
	
	void				Roll();

	void				PositionRock_Check();
	void				Key_State();
 	void				Keybord_OffSet();



	bool				RocketTime();
	bool				ChargeShotTime();

	virtual void		TimeCheck();
	void				DeadEffect();
	bool				SubWeapon_Check();
	void				SubWeapon_Select();

	void				SubWeapon_Charge();




private:
	float				m_fRocketTime[4];
	float				m_fRocketSpeed;
	int					m_iMaxRocket;

	bool				m_bRoll;
	float				m_fRollTime;
	float				m_fRollCoolSpeed;
	
	bool				m_bMega;

	bool				m_bAuto;
	//프레싱이 일정시간 안되면 로켓 -> 일정시간 후엔 차지샷 
	//
	float				m_fChargeTime; //차지 시간 측정
	
	float				m_fChargeCoolTime;	// 
	float				m_fChargeSpeed;	//차지 주기를  측정 ->공격속도 같은

	


	PLAYER::STATE		m_eState;
	PLAYER::STATE		m_ePreState;

	BULLET::SUBWEAPON	m_eSubWeaponState;
	

	CGameObject*		m_pBurner;
	
	CGameObject*		m_pChargeBeam;
	wstring wstrSubWeapon;
	wstring wstrChargeWeapon;

};
