#pragma once
#include "GameObject.h"
class CBackGround :
	public CGameObject
{
private:
	explicit CBackGround();
public:
	virtual ~CBackGround();
public:
	static	CGameObject* Create(const PLACEMENT*	_pPlacement);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;		//gui 와 함께 red로 바꿔주기 위해서

	virtual void WriteMatrix();

protected:

	// CGameObject을(를) 통해 상속됨
};

