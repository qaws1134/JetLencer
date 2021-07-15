#pragma once
#include "Ui.h"
class CFlip :
	public CUi
{
private:
	explicit CFlip();
public:
	virtual ~CFlip();
public :
	static CGameObject* Create(_vec3 _vPos, bool _bYflip);


	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	
public :
	void Set_PlayerState(PLAYER::STATE _ePlayerState) { m_ePlayerState = _ePlayerState; }
	void Set_WeaponState(PLAYER::STATE _eWeaponState) { m_ePlayerWeaponState = _eWeaponState; }
	void Set_YFlip(bool _bYflip) { m_bYflip = _bYflip; }
	void Set_OverHeat(bool _bOverHeat) { m_bOverHeat = _bOverHeat; }
public :
	void VertUpdate();
	void HoriUpdate();
	void Update_Pos();
	void Update_Size();
	void Init_Create();

private :
	CGameObject* m_pAfterBurner;
	
	
	CGameObject* m_pAfterBurner_Red;
	
	CGameObject* m_pFlip_Glitch;
	CGameObject* m_pFlip_Red;

	CGameObject* m_pArrow;
	CGameObject* m_pArrow_Red;
	
	
	CGameObject* m_pVertSpeed;
	CGameObject* m_pVertSpeed_Red;
	
	CGameObject* m_pOverHeat_Plate;
	CGameObject* m_pOverHeat_Plate_Red;
	CGameObject* m_pOverHeat_Text;
	CGameObject* m_pOverHeat_Text_Red;

	CGameObject* m_pSpecial_Charge;
	CGameObject* m_pSpecial_Plate;
	CGameObject* m_pSpecial_Plate_Red;
	CGameObject* m_pSpecial_Reload_Glitch;
	CGameObject* m_pSpecial_Reload_Bar;
	CGameObject* m_pSpecial_Reload_Ready_Bar;
	CGameObject* m_pSpecial_Reload_Ready_Backplate;
	CGameObject* m_pSpecial_Reload_Ready_Backplate_Red;



	PLAYER::STATE m_ePlayerState;
	PLAYER::STATE m_ePlayerWeaponState;

	float m_fRedTime;
	bool m_bYflip;
	bool m_bOverHeat;
	bool m_bRed;
	bool m_bReload;
	bool m_bStart;
	bool m_bGlitch;

	float m_fAfterBurnerX ;
	float m_fAfterBurnerY ;
	float m_fAfterBurnerRedY ;

	float m_fArrowX ;

	float m_fVertX ;
	float m_fVertY ;

	float m_fSpecial_ChargeX ;
	float m_fSpecial_ChargeY ;

	float m_fSpecial_PlateX ;
	float m_fSpecial_PlateY ;

	float m_fSpecial_Reload_BarX ;
	float m_fSpecial_Reload_BarY ;

	float m_fSpecial_Reload_PlateX;
	float m_fSpecial_Reload_PlateY;

	float m_fOverHeat_PlateX;
	float m_fOverHeat_PlateY;
};

