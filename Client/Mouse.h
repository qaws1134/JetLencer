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
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public :
	static CGameObject* Create();
public :
	_vec3 Get_MousePos() {return m_vMouse; }
	virtual void State_Change() override;

private:
	_vec3 m_vMouse;

	// CGameObject��(��) ���� ��ӵ�
};

#endif
