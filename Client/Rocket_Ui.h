#pragma once
#include "Ui.h"
class CRocket_Ui :
	public CUi
{
private:
	explicit CRocket_Ui();
public:
	virtual ~CRocket_Ui();
	static CGameObject* Create(_vec3 _vPos);

	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	void	Start_Rocket(int _iIdx) { m_bRocket[_iIdx] = true; }
	void	End_Rocket(int _iIdx) { m_bRocket[_iIdx] = false; }
	void	Update_Pos();
	void	Update_Size();
	void	Set_Red(bool _bRed) { m_bUiRed = _bRed; }
	void	UnReady() { m_bUnReady = true; }

	CGameObject* m_pRocket_Plate_Red;
	CGameObject* m_pRocket[4];
	CGameObject* m_pRocket_Red[4];


	bool m_bUnReady;
	int m_iIdx;
	bool m_bRocket[4];
	bool m_bPlate;
	bool m_bUiRed; 
	bool m_bRed;
	float m_fRedTime;
};

