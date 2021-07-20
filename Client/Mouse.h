#pragma once
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "GameObject.h"
class CMouse final	:public CGameObject
{

public:
	explicit CMouse();
	virtual ~CMouse();

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public :
	static CGameObject* Create();
public :
	_vec3 Get_MousePos();
	virtual void State_Change() override;
	virtual void WriteMatrix();
	void	 Set_MouseState(MOUSE::STATE _eMouse) { m_eMouse = _eMouse; }
	void	Set_HitTime() { m_fHitTime = 0.f; }

private:
	
	bool IdleTime();
	void SizeCheck();
	_vec3 m_vMouse;
	MOUSE::STATE m_eMouse;
	MOUSE::STATE m_ePreMouse;

	float m_fSizeCheckTime;
	float m_fSizeCheckSpeed;

	float m_fHitTime;
	float m_fHitSpeed;



	_vec3 m_vPrePos;
	CGameObject* pHit;
	// CGameObject을(를) 통해 상속됨
};

#endif
