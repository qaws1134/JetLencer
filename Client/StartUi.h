#pragma once
#include "Ui.h"
class CStartUi :
	public CUi
{
private:
	CStartUi();
public:
	virtual ~CStartUi();


	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
};

