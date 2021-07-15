#pragma once
#include "GameObject.h"
class CUi :
	public CGameObject
{
public:
	explicit CUi();
	virtual ~CUi();
public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual int		Update_GameObject() PURE;
	virtual void	Late_Update_GameObject()override;
	virtual void	Render_GameObject() override;
	virtual void	Release_GameObject() override;
	virtual void	State_Change() PURE;

public :
	void			Set_Frame(const ANIMATION * _tAnimationInfo);
	void			Set_Render(bool _bRender) { m_bRender = _bRender; }
	void			Set_FrameSpeed(float _fSpeed) { m_tFrame.fFrameSpeed = _fSpeed; }

public :
	virtual void	WriteMatrix();
	void			Select_Frame(float _fFrame) { m_tFrame.fStartFrame = _fFrame; }
	void			Set_Offset(float _fOffSet) { m_fOffset = _fOffSet; }

	float			m_fOffset;
	bool			m_bRender;
};

