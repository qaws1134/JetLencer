#pragma once
#include "Scene.h"
class CSelectStage :public CScene
{
private:
	explicit CSelectStage();
public:
	virtual ~CSelectStage();

	static CScene* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
public :
	void Key_Check();

public :
	vector<CGameObject*> m_vecUi;

	CGameObject* m_pPointer;
	int m_iStageIdx;
	bool m_bDead;

};

