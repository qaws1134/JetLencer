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
	static CGameObject* Create(wstring _wstrText, TEXT::SIZE _eSize);
	static CGameObject* Create(wstring _wstrText);
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual int Update_GameObject() override;
	virtual void WriteMatrix();
public:
	void	Set_Text(wstring _wstrText) { m_wstrText = _wstrText; }
	void	Set_Point(float  _fPoint) { m_fPoint = _fPoint; }
	void	Set_iPoint(int  _iPoint) { m_iPoint = _iPoint; }
	void	Set_TextSize(TEXT::SIZE _eTextSize) { m_eTextSize = _eTextSize; }

	wstring m_wstrText;
	float m_fPoint;
	int m_iPoint;

	TEXT::SIZE m_eTextSize;



	// CUi을(를) 통해 상속됨
};

