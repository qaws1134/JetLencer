#pragma once
#include "GameObject.h"
class CParticle :
	public CGameObject
{


private:
	CParticle();
public:
	virtual ~CParticle();
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, EFFECT::TYPE _eEffectType);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;

	
	void Set_EffectType(EFFECT::TYPE _eEffectType) { m_eEffectType = _eEffectType; }
	void Frame_Change();
	void Move();
	void DeadEffect();
	void InitParticle();

	EFFECT::TYPE m_eEffectType;
	float m_fTime;
	float m_fSize;

	float m_fSpawnTime;
	float m_fSpawnSpeed;

	float m_fDeadTime;
	float m_fDeadSpeed;

	float m_fMaxSpeedOffSet;
	float m_fReduceSize;
	float m_fMinSize;
};

