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



public:
	void	Ui_DistanseState(CGameObject* _pUiTarget);
	void	Ui_DirState(CGameObject* _pUiTarget);


public :
	void	Set_JetState(ENEMY::STATE _eJetState) { m_eJetState = _eJetState; }
	void	Set_UiState(ARROW::UI _eUiState) { m_eUiState = _eUiState; }
	bool	Get_Zoom() { return m_bZoom; }
	void	EnemyBurnerSet();
public :
	ENEMY::STATE m_eJetState;
	ENEMY::STATE m_ePreJetState;

	ARROW::UI m_eUiState;
	CGameObject* m_pArrow_Offscreen;


	_vec3 m_vResultDir;
	float m_fOffsetAngle;
	float m_fTargetAngle;
	bool	m_bZoom;

	// CJet을(를) 통해 상속됨
};


#endif // !__JET_ENEMY_H__
