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


	// CJet��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;

	// CJet��(��) ���� ��ӵ�
	virtual void State_Change() override;
	virtual void Ai_State() override;

};

