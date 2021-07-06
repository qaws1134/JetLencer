#pragma once
#include "GameObject.h"
class CEffect :
	public CGameObject
{
private:
	explicit CEffect();
public:
	virtual ~CEffect();
public:
	static	CGameObject* Create(const ANIMATION*	_tAnimationInfo, D3DXVECTOR3 _vPos, bool _bFrameStart, EFFECT::TYPE _eEffectType);
	static	CGameObject* Create(const PLACEMENT*	_tPlacementInfo, D3DXVECTOR3 _vPos );

public :
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public :
	void Set_Prefab(const ANIMATION * _pPrefab) { m_pAnimation = _pPrefab; }
	void Set_Prefab(const PLACEMENT * _pPrefab) { m_pPlacement = _pPrefab; }
	void Set_FrameStart(bool _bFrameStart) { m_bFrameStart = _bFrameStart; }
	void Set_Type(EFFECT::TYPE _eEffectType) { m_eEffectType = _eEffectType; }
	void Set_Frame(const ANIMATION*	_tAnimationInfo);
	void Set_Frame( const PLACEMENT*	_tPlacementInfo);
public :
	virtual void State_Change() override;
	void Frame_Change();
	void Setting_Size();



protected:
	//const ANIMATION*	m_pAnimationInfo;
	//const OBJECTINFO*	m_pObjectInfo;
	//const PLACEMENT*	m_pPlacementInfo;
	EFFECT::TYPE		m_eEffectType;
	bool				m_bFrameStart;
	float				m_fSize;
	float				m_fReduce;
	// CGameObject을(를) 통해 상속됨
};

