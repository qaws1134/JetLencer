#pragma once

#include "Jet_Enemy.h"
class CJet_BulletSplash :public CJet_Enemy
{
private:
	explicit CJet_BulletSplash();
public:
	virtual ~CJet_BulletSplash();

	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos);
	// CJet_Enemy을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Ai_State() override;
	virtual void DeadEffect();
};

