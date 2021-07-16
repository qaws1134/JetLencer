#pragma once
#include "Ui.h"
class CHp :
	public CUi
{
private:
	explicit CHp();
public:
	virtual ~CHp();
public:
	static CGameObject* Create(_vec3 _vPos);

	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;

	void	Update_Pos();
	void	Update_Size();
	void	Set_Red(bool _bRed) { m_bUiRed = _bRed; }
	void	Set_Hp(int _iHp) { m_iHp = _iHp; }
	void	End_Super(bool _bSuper) { m_bSuper = _bSuper; }
	void	Set_State(PLAYER::STATE _eState) { m_eState = _eState; }

	bool m_bUiRed;
	bool m_bRed;
	float m_fRedTime;
	int m_iHp;
	int m_iPreHp;
	bool m_bSuper;
	PLAYER::STATE m_eState;
	CGameObject* m_pHp_Plate_Red;
	CGameObject* m_pHp_Glitch;
	CGameObject* m_pHp;
	CGameObject* m_pHp_Red;




};

