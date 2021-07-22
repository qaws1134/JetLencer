#pragma once
#ifndef __ROCKET_H__
#define __ROCKET_H__
#include "Bullet.h"

class CRocket  :
	public CBullet
{
private:
	explicit CRocket();
public:
	virtual ~CRocket();

public:
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed);
	
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
	virtual void Late_Update_GameObject() override;

public :
	void	Accel(_vec3 vDir, float _fAccel, float _fMaxSpeed, bool bGravity);
	bool	Get_TargetOn() { return m_bTargetOn; }

	void	Set_UiState(ARROW::UI _eUiState) { m_eUiState = _eUiState; }
	void	Ui_DistanseState(CGameObject* _pUiTarget);
	void	Ui_DirState(CGameObject* _pUiTarget);

	ARROW::UI m_eUiState;
	CGameObject* m_pArrow_Offscreen;


private:
	CGameObject* m_pMarkerOveray;
	float m_fTargetTime;
	float m_fTargetSpeed;
	bool m_bTargetOn;

};
#endif // !__ROCKET_H__
