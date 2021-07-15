#pragma once
#include "Gui.h"
class CViewText :
	public CUi
{
private:
	explicit CViewText();
public:
	virtual ~CViewText();

	static CGameObject* Create(_vec3 vPos, wstring _wstrText);

	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;

public:
	void	Set_Text(wstring _wstrText) { m_wstrText = _wstrText; }
	void	Set_Point(float  _fPoint) { m_fPoint = _fPoint; }

	wstring m_wstrText;
	float m_fPoint;

	// CUi을(를) 통해 상속됨
	virtual int Update_GameObject() override;
};

