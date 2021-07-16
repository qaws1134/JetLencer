#pragma once
#include "Ui.h"
class CDmgGrid :
	public CUi
{
private:
	explicit CDmgGrid();
public:
	virtual ~CDmgGrid();

	static CGameObject* Create();


	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	void Set_Red(bool _bRed) { m_bUiRed = _bRed; }
	void Set_Green(bool _bGreen) { m_bUiGreen = _bGreen; }
	void Remove_Grid();
	bool m_bUiRed;
	bool m_bUiGreen;
	float m_fGridTime;
	int m_iRenderIdx;

	vector<CGameObject*> m_vecGrid;
};

