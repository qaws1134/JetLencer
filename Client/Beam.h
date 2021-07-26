#pragma once
#include "Bullet.h"
class CBeam :
	public CBullet
{
private:
	explicit CBeam();
public:
	virtual ~CBeam();

	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed, BULLET::SUBWEAPON _eSubId);
	static CGameObject * Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos, float _fAngle, _vec3 _vSpeed,CGameObject* _pTarget,bool _bBoss);
	// CBullet을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual void State_Change() override;
	virtual void Move() override;
	virtual void DeadEffect() override;
	void Set_BossBeam(bool _bBoss) { m_bBoss = _bBoss; }
	void Boss_State();



	void InitState();
	void Frame_Change();
	bool m_bBoss;
	bool bColSet;
	float m_fColSize;
	bool m_bEffect;
	vector<CGameObject*> m_vecDeadEffect; 
	CGameObject* m_pFlash;

	// CBullet을(를) 통해 상속됨
	virtual void Late_Update_GameObject() override;
};

