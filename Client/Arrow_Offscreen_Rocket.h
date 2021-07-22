#pragma once
#include "Ui.h"
class CArrow_Offscreen_Rocket :
	public CUi
{
private:
	explicit CArrow_Offscreen_Rocket();
public:
	virtual ~CArrow_Offscreen_Rocket();


	static	CGameObject* Create();


	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	void	Set_ArrowState(ARROW::UI _eArrowState) { m_eArrowState = _eArrowState; }
	
	void	ArrowDistance_Change();
	void	ArrowPosition_Change();
	void	ArrowColor_Change();


	ARROW::UI m_eArrowState;
	ARROW::UI m_ePreArrowState;

	float m_fColorTime;
	float m_fColorSpeed;
	bool m_bColor;




};

