#pragma once
#ifndef __JET_ENEMY_H__
#define __JET_ENEMY_H__

#include "Jet.h"
class CJet_Enemy :public CJet
{
public:
	explicit CJet_Enemy();
	virtual ~CJet_Enemy();

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual int Update_GameObject() override;

	virtual void State_Change() PURE;
	virtual void Ai_State()PURE;
	virtual void DeadEffect() override;

	float m_fOffsetAngle;
	float m_fTargetAngle;
	// CJet��(��) ���� ��ӵ�
};


#endif // !__JET_ENEMY_H__
