#pragma once
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:

public:
	virtual HRESULT		Ready_GameObject()PURE;
	virtual int			Update_GameObject()PURE;
	virtual void		Late_Update_GameObject()PURE;
	virtual void		Render_GameObject()PURE;
	virtual void		Release_GameObject()PURE;


public:
	virtual HRESULT		Set_Texture();
	virtual HRESULT		Set_Texture(const wstring & wstrObjectKey);
	virtual HRESULT		Set_Texture(const wstring & wstrObjectKey, const wstring& wstrStateKey);
public :
	void				Set_Pos(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; };
	void				Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	void				Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void				Set_Speed(_vec3 _vSpeed) { m_vVelocity = _vSpeed; }
	void				Set_CenterPos(_vec3 vPos);

	void				Set_Prefab(const OBJECTINFO* _pPrefab) { m_pObjectInfo = _pPrefab; }
	void				Set_Placement(const PLACEMENT* _pPlacement) { m_pPlacement = _pPlacement; }
	void				Set_Animation(const ANIMATION* _pAnimation) { m_pAnimation = _pAnimation; }

public:
	const INFO&			Get_ObjInfo()const { return m_tInfo; }
		  FRAME&		Get_Frame() { return m_tFrame; }
	const RENDERID::ID&	Get_RenderId() const { return m_eRenderId; }

public :
	_vec3				Get_Velocity() { return m_vVelocity; }
	float				Get_Angle() { return m_fAngle; }
public :
	virtual void		State_Change()PURE;
	virtual void		WriteMatrix();
	virtual void		Angle_Change();
	void				TargetAngle_Check();

protected:

	const TEXINFO*		m_pTexInfo = nullptr;
	const OBJECTINFO*	m_pObjectInfo;
	const PLACEMENT*	m_pPlacement;
	const ANIMATION*	m_pAnimation;
	FRAME				m_tFrame;
	RENDERID::ID		m_eRenderId;
	ANGLE::STATE		m_eAngleState;
	
	float				m_fCenterX;
	float				m_fCenterY;

	CGameObject*		m_pTarget;
	INFO				m_tInfo; 
	bool				m_bDead;
	float				m_fAngle = 0.f;
	float				m_fSpeed;
	float				m_fAngleSpeed; 


	_vec3				m_vVelocity;
	float				m_fAccel;

	_vec3				m_vGravity;
	float				m_fRegistPower;


};

