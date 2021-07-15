#pragma once
#include "Ui.h"
class CGui :
	public CUi
{
private:
	explicit CGui();
public:
	virtual ~CGui();

public :
	static CGameObject *Create(const PLACEMENT * _pPlacement);
	static CGameObject *Create(UI::TYPE _eUiType);

public :
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
public:
	void	Set_UiType(UI::TYPE _eUiType)	{ m_eUiType = _eUiType; }
	void	Set_Action(bool _bAction)		{ m_bAction = _bAction; }
	void	Set_Start(bool _bStart)			{ m_bStart  = _bStart; }
	void	Set_TargetPos(_vec3 _vPos)		{ m_vTargetPos = _vPos;}


public:
	void	InitGui();
	void	Frame_Change();
private:
	UI::TYPE m_eUiType;
	bool m_bLoop;
	bool m_bAction;
	bool m_bStart;
	_vec3 m_vTargetPos;
	float m_fIncrease;
	// CUi을(를) 통해 상속됨
};

