#pragma once
#include "Gui.h"
class CArrow_Offscreen :
	public CUi
{
private:
	explicit CArrow_Offscreen();
public:
	virtual ~CArrow_Offscreen();

	static	CGameObject* Create(UI::ICON _eIcon);

	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;

	void				Set_Icon(UI::ICON _eIcon) { m_eIcon = _eIcon; }
	void				Set_ArrowState(ARROW::UI _eArrowState) { m_eArrowState = _eArrowState; }
	void				Set_Distance(float _fDis) { m_fDis = _fDis; }

	void				ArrowDir_Change();
	void				ArrowDistance_Change();
	void				ArrowPosition_Change();
	void				ArrowAlpha_Change();


	CGameObject*		m_pArrow_Offscreen1;
	CGameObject*		m_pArrow_Offscreen2;
	CGameObject*		m_pArrow_Offscreen_Rect;
	CGameObject*		m_pArrow_Icon;
	CGameObject*		m_pArrow_Marker_Rocket;
	UI::ICON m_eIcon;

	float m_fDis;

	float m_fPreFrame;
	ARROW::UI m_eArrowState;
	ARROW::UI m_ePreArrowState;

	float m_fColorTime;
	float m_fColorSpeed;
	bool  m_bColorChange;
	_vec3 m_vecArrow;
	_vec3 m_vecRect;

	_vec3 m_vecArrowResult;
	_vec3 m_vecRectResult;

	// CUi을(를) 통해 상속됨
	virtual int Update_GameObject() override;
};

