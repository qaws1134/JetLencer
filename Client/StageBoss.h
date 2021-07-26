#pragma once
#include "Scene.h"
class CStageBoss :
	public CScene
{
private:
	explicit CStageBoss();
public:
	virtual ~CStageBoss();

	static CScene* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

	CGameObject* m_pBoss;
	CGameObject* m_pStart_End;

	vector<CGameObject*> m_vecStageStart;

	int m_iStageUiIndex;
	bool m_bSoundStart;
	bool m_bStart;
	bool m_bEnd;
	bool m_bBoss;


	float m_fStartTime;
	float m_fStartSpeed;

	float m_fEndTime;
	float m_fEndSpeed;

};

