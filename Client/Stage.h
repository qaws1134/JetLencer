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
	static CScene* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public:

	CGameObject* m_pOject1;
	CGameObject* m_pOject2;
	CGameObject* m_pUI3;

	vector<CGameObject*> m_vecStageStart;
	int m_iStageUiIndex;

	float m_fStartTime;
	float m_fStartSpeed;

};

