#pragma once
#include "Jet_Enemy.h"
class CSerpentObject :
	public CJet_Enemy
{
public:
	explicit CSerpentObject();
	virtual ~CSerpentObject();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() PURE;
	virtual int Update_GameObject() override;
	virtual void State_Change() PURE;
	virtual void Select_Armor() PURE;


public:
	COMBAT	Get_CombatInfo() { return m_tCombatInfo; }
	bool	Get_Break(){return m_bBreak;}
	void	Set_Pattern(SERPENT::PATTERN _ePattern) { m_eSerpentPattern = _ePattern; }
	void	Set_AniStart() { m_fAnimationStart = 0.f; }
	void	Set_Attack_End(bool _Attack_end) { m_bAttack_End = _Attack_end; }
	bool	Get_Attack_End() { return m_bAttack_End; }
	bool	Get_Crash() { return m_bCrash; }

protected:
	virtual void DeadEffect();
	virtual void InitJet();

	virtual void Ai_State() PURE;
	void Animation_Change(const ANIMATION* _pAnimation);
	void Move();


	bool m_bCrash;
	bool m_bBreak;
	SERPENT::PATTERN m_eSerpentPattern;
	COMBAT m_tCombatInfo;

	float m_fAnimationStart;
	float m_fAnimationMax;
	float m_fAnimationSpeed;
	
	float m_fPatternTime;
	float m_fPatternSpeed;

	float m_fSpawnTime;
	float m_fSpawnSpeed;

	int iMaxHp;

	bool m_bAnimation;
	bool m_bAttack;
	bool m_bAttack_End;

	const ANIMATION* m_AniCrash;
	const ANIMATION* m_AniFrame;
	const ANIMATION* m_AniIdle;
};

