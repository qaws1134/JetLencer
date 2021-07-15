#pragma once
#include "Jet_Enemy.h"
class CJet_Normal :public CJet_Enemy
{
private:
	explicit CJet_Normal();
public:
	virtual ~CJet_Normal();
public:
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos);


	// CJet을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;

	// CJet을(를) 통해 상속됨
	virtual void State_Change() override;
	virtual void Ai_State() override;

};

