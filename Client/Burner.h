#pragma once
#include "GameObject.h"
class CBurner :
	public CGameObject
{
private:
	explicit CBurner();
public:
	virtual ~CBurner();

public :
	static CGameObject*	Create();

public :
	virtual HRESULT		Ready_GameObject() override;
	virtual int			Update_GameObject() override;
	virtual void		Late_Update_GameObject() override;
	virtual void		Render_GameObject() override;
	virtual void		Release_GameObject() override;
	virtual void		WriteMatrix()override;
public :
	virtual void		State_Change() override;
	void				Set_BurnerState(BURNER::STATE _eState) { m_eBurnerState = _eState; }
	void				Frame_Change();
public :
	bool				m_bRender;
	bool				m_bLoop;
	BURNER::STATE		m_eBurnerState;
	BURNER::STATE		m_ePreBurnerState;
};

