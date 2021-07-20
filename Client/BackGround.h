#pragma once
#include "GameObject.h"
class CBackGround :
	public CGameObject
{
private:
	explicit CBackGround();
public:
	virtual ~CBackGround();
public:
	static	CGameObject* Create(const PLACEMENT*	_pPlacement);

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	virtual void State_Change() override;		//gui 와 함께 red로 바꿔주기 위해서


	virtual void WriteMatrix();

	void Set_Render(bool _bBossLaser) { m_bBossLaser = _bBossLaser; }
	bool Get_Render() { return m_bBossLaser; }
	void Set_Hit(bool _bHit) { m_bHit = _bHit; }
	bool Get_Hit() { return m_bHit; }
protected:
	float m_fSplashTime;
	_vec3 vMaxScale;
	_vec3 vMinScale;

	bool m_bBossLaser;
	bool m_bHit;
	int m_iAlpha;
	
	// CGameObject을(를) 통해 상속됨
};

