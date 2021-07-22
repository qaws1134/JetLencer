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
	void	Set_Timer(float _fTimer) { m_fTimer = _fTimer; }
	void	Set_Red(bool _bRed) { m_bRed = _bRed; }
	void	Set_Green(bool _bGreen) { m_bGreen = _bGreen; }
	void	Set_HpSize(float _fHpRatio) { m_fTargetSize = m_fMaxSize *_fHpRatio; }

public :
	bool	Get_Start() { return m_bStart; }
public:
	void	InitGui();
	void	Frame_Change();
private:
	float m_fTimer;
	float m_fTime;
	float m_fReloadTime;
	float m_fMaxSize;
	float m_fTargetSize;


	UI::TYPE m_eUiType;
	bool m_bLoop;
	bool m_bAction;
	bool m_bStart;
	bool m_bRed;
	bool m_bGreen;
	_vec3 m_vTargetPos;
	float m_fIncrease;
	// CUi을(를) 통해 상속됨
};

