#pragma once
#include "Ui.h"
class Rocket_Ui :
	public CUi
{
public:
	Rocket_Ui();
	virtual ~Rocket_Ui();

	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;

	CGameObject* m_pRocket_Plate;
	CGameObject* m_pRocket[4];
	CGameObject* m_pRocket_Red[4];


};

