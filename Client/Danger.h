#pragma once
#include "Ui.h"
class CDanger :
	public CUi
{
private:
	explicit CDanger();
public:
	virtual ~CDanger();
public :
	static CGameObject* Create(_vec3 _vPos);

	// CUi을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void State_Change() override;
	
	void Set_DangerState(DANGER::STATE _eState) { m_eDangerState = _eState; }
	bool Get_Danger_End() { return m_bStart; }


	CGameObject* m_pDanger_Idle;
	CGameObject* m_pDanger_Crit;
	CGameObject* m_pDanger_Start;
	CGameObject* m_pDanger_End;

	DANGER::STATE m_eDangerState;
	DANGER::STATE m_ePreDangerState;

	bool m_bStart;

};

