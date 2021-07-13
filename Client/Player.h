#pragma once
#include "Jet.h"
class CPlayer :public CJet
{
private:
	explicit CPlayer();
public:
	virtual ~CPlayer();
public:
	// CGameObject��(��) ���� ��ӵ�
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
	//�������� �����ð� �ȵǸ� ���� -> �����ð� �Ŀ� ������ 
	//
	float				m_fChargeTime; //���� �ð� ����
	
	float				m_fChargeCoolTime;	// 
	float				m_fChargeSpeed;	//���� �ֱ⸦  ���� ->���ݼӵ� ����

	


	PLAYER::STATE		m_eState;
	PLAYER::STATE		m_ePreState;

	BULLET::SUBWEAPON	m_eSubWeaponState;
	

	CGameObject*		m_pBurner;
	
	CGameObject*		m_pChargeBeam;
	wstring wstrSubWeapon;
	wstring wstrChargeWeapon;

};
