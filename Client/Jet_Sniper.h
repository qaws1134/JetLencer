#pragma once
#include "Jet_Enemy.h"
class CJet_Sniper :
	public CJet_Enemy
{
private:
	explicit CJet_Sniper();
public:
	virtual ~CJet_Sniper();


static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos);
public :
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Ai_State() override;

	virtual void DeadEffect();


private:
	vector<CGameObject*>m_vecArrow;
	bool	m_bAttack;
};

