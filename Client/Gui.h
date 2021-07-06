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

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;
protected:

	// CGameObject을(를) 통해 상속됨
};

