#pragma once
#include "Jet_Enemy.h"
class CJet_Rocket :
	public CJet_Enemy
{
private:
	explicit CJet_Rocket();
public:
	virtual ~CJet_Rocket();


	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos);


	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Ai_State() override;

};

