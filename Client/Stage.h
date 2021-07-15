#pragma once
#include "Scene.h"

class CStage final :
	public CScene
{
private:
	explicit CStage();
public:
	virtual ~CStage();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:
	static CScene* Create(); 

	CGameObject* m_pOject1;
	CGameObject* m_pOject2;
	CGameObject* m_pUI3;


};

