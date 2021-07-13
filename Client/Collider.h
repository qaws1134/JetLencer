#pragma once
#include "GameObject.h"
class CCollider :
	public CGameObject
{

public:
	CCollider();
	virtual ~CCollider();

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_ColObject();
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;
	virtual int Update_GameObject() override;


public :
	virtual void WriteMatrix()PURE;
public :
	void				Set_CollisionSize(float fSphereSize) { m_fSphereSize = fSphereSize; }
	void				Set_ColRect(_vec3 LeftTop, _vec3 RightBottom);
	void				Set_ColId(COLLIDER::ID eColId) { m_eColId = eColId; }
	void				Set_OffSetDir(_vec3 vOffsetDir) { m_vOffsetDir = vOffsetDir; }
	void				Set_CombatInfo(COMBAT _tCombatInfo) { m_tCombatInfo = _tCombatInfo; }
	void				Set_Dmg(int _iAtk) { m_tCombatInfo.iHp += _iAtk; }



	float				Get_ColSphereSize() { return m_fSphereSize; }
	RECT				Get_ColRect() { return m_ColRect; }
	COLLIDER::ID		Get_ColId() { return m_eColId; }
	
	COMBAT				Get_CombatInfo() { return m_tCombatInfo; }

protected:
	COLLIDER::ID		m_eColId;
	_vec3				m_vOffsetDir;
	float				m_fSphereSize;
	RECT				m_ColRect;
	COMBAT				m_tCombatInfo;

};


