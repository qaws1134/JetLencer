#pragma once
#include "GameObject.h"
class CGui :
	public CGameObject
{
private:
	explicit CGui();
public:
	virtual ~CGui();
public:
	static	CGameObject* Create(const PLACEMENT*	_pPlacement);
	static	CGameObject* Create(_vec3 vPos,wstring _wstrText);


	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;

	virtual void WriteMatrix();
public:
	void	Set_Text(wstring _wstrText) { m_wstrText = _wstrText;  }
	void	Set_Point(float  _fPoint) { m_fPoint = _fPoint; }

	wstring m_wstrText;
	float m_fPoint;
};

