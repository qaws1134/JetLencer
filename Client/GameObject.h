#pragma once
class CCollider;
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
	void				Set_Dir(const _vec3& vDir) { m_tInfo.vDir = vDir; }

	void				Set_Prefab(const OBJECTINFO* _pPrefab) { m_pObjectInfo = _pPrefab; }
	void				Set_Placement(const PLACEMENT* _pPlacement) { m_pPlacement = _pPlacement; }
	void				Set_Animation(const ANIMATION* _pAnimation) { m_pAnimation = _pAnimation; }
	void				Set_RenderId(RENDERID::ID _eRenderId) { m_eRenderId = _eRenderId; }
	void				Set_Dead(bool bDead) { m_bDead = bDead; }
	void				Set_DeadEffect(bool bDeadEffect) { m_bDeadEffect = bDeadEffect; }
	void				Set_Color(MATCOLOR _tColor) { m_tColor = _tColor; }
	void				Set_Size(_vec3 _vSize) { m_tInfo.vSize = _vSize; }
	void				Set_Alpha(int iAlpha) { m_tColor.iAlpha = iAlpha; }
	void				Set_ColorTime() { m_fColorTime = 0.f; }

	void				Set_TrueMod(bool _bTrueMod) { m_bTrueMod = _bTrueMod; }
	void				Set_AllTrueMod(bool _bAllTrueMoad) { m_bAllTrueMod = _bAllTrueMoad; }

public:
	const INFO&			Get_ObjInfo()const { return m_tInfo; }
		  FRAME&		Get_Frame() { return m_tFrame; }
	const RENDERID::ID&	Get_RenderId() const { return m_eRenderId; }
	CCollider*			Get_ColVec(int i) {vector<CCollider*>::iterator iter =  m_vecCollider.begin(); return iter[i];}

	int					GetColSize() { return m_vecCollider.size(); }
	_vec3				Get_Velocity() { return m_vVelocity; }
	float				Get_Angle() { return m_fAngle; }
	CGameObject*		Get_Target() { return m_pTarget; }
	MATCOLOR			Get_Color() { return m_tColor; }
	const TEXINFO*		Get_Texture() const { return m_pTexInfo; }

	bool				Get_TrueMod(){ return m_bTrueMod; }


public :
	virtual void		State_Change()PURE;
	virtual void		WriteMatrix();

	void				TargetAngle_Check();

protected:

	const TEXINFO*		m_pTexInfo = nullptr;
	const OBJECTINFO*	m_pObjectInfo;
	const PLACEMENT*	m_pPlacement;
	const ANIMATION*	m_pAnimation;
	FRAME				m_tFrame;
	RENDERID::ID		m_eRenderId;
	ANGLE::STATE		m_eAngleState;
	

	float				m_fColorTime;
	float				m_fColorSpeed;


	float				m_fCenterX;
	float				m_fCenterY;

	CGameObject*		m_pTarget;
	INFO				m_tInfo; 
	bool				m_bDead;

	float				m_fAngle;
	float				m_fAngle_per_Frame;
	float				m_fAngleOffset;
	float				m_fAngleSpeed; 

	float				m_fSpeed;
	float				m_fMaxSpeed;


	_vec3				m_vCross;
	_vec3				m_vTarget_Dir;
	_vec3				m_vVelocity;
	float				m_fAccel;

	_vec3				m_vGravity;
	float				m_fRegistPower;

	vector<CCollider*>  m_vecCollider;
	MATCOLOR			m_tColor;

	bool				m_bDeadEffect;
	bool				m_bCenter;

	bool				m_bTrueMod;
	bool				m_bAllTrueMod;
};

