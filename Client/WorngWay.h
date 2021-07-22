#pragma once
#include "Ui.h"
class CWorngWay :
	public CUi
{
private:
	explicit CWorngWay();
public:
	virtual ~CWorngWay();

	static CGameObject* Create(_vec3 _vPos, bool _bFlip);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	void Set_Flip(bool _bFlip) { m_bFlip = _bFlip; }
	void Set_WornWayState(WORNWAY::STATE _eWornWayState) { m_eWornWayState = _eWornWayState; }


	CGameObject* m_pWrongWay_Idle;
	CGameObject* m_pWrongWay_Start;
	CGameObject* m_pWrongWay_End;



	WORNWAY::STATE m_eWornWayState;
	WORNWAY::STATE m_ePreWornWayState;

	bool m_bFlip;
	bool m_bEnd;
};

