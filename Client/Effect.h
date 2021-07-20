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
	static	CGameObject* Create(EFFECT::TYPE _eEffectType);
	static	CGameObject* Create(const ANIMATION* _tAnimation, D3DXVECTOR3 _vPos);
	static	CGameObject* Create(const PLACEMENT*	_tPlacementInfo, D3DXVECTOR3 _vPos );
	static	CGameObject* Create(EFFECT::TYPE _eEffectType, D3DXVECTOR3 _vPos, bool _bFrameStart);
	static	CGameObject* Create(EFFECT::TYPE _eEffectType, D3DXVECTOR3 _vPos, bool _bFrameStart,float _fSize);

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
	void Set_EffectType(EFFECT::TYPE _eEffectType) { m_eEffectType = _eEffectType; }
	void Set_Size(float _fSize) { m_fSize = _fSize; }
	void Set_Frame(const ANIMATION*	_tAnimationInfo);
	void Set_Frame( const PLACEMENT*	_tPlacementInfo);
	void Set_Frame(FRAME& _tFrame);
	void Set_Loop(bool _bLoop) { m_bLoop = _bLoop; }
	void Set_ReduceTime(float _fTime) { m_fReduceTime = _fTime; }


public :
	virtual void State_Change() override;
	void Frame_Change();
	void InitEffect();
	void Size_Reduce();


protected:
	//const ANIMATION*	m_pAnimationInfo;
	//const OBJECTINFO*	m_pObjectInfo;
	//const PLACEMENT*	m_pPlacementInfo;

	float				m_fDelTime;
	EFFECT::TYPE		m_eEffectType;
	bool				m_bFrameStart;
	bool				m_bLoop;
	float				m_fSize;
	float				m_fReduce;


	float				m_fReduceTime;
	float				m_fReduceDelay;
	// CGameObject을(를) 통해 상속됨
};

