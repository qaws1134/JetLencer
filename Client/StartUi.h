#pragma once
#include "Ui.h"
class CStartUi :
	public CUi
{
private:
	CStartUi();
public:
	virtual ~CStartUi();


	// CUi��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
};

