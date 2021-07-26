#pragma once
#include "Scene.h"
class CStage3 :
	public CScene
{
private:
	explicit CStage3();
public:
	virtual ~CStage3();

	static CScene* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	void Set_StartStage() { m_bStart = true; }

private:
	float m_fSpawnTime;
	float m_fSpawnSpeed;

	float m_fWaveTime;
	float m_fWaveSpeed;

	bool m_bStart;
	bool m_bEnd;
	int m_iSpawnCount;
	float m_fSecond;
	list<wstring> m_listEnemy[3];
	vector<_vec3> m_vecSpawnPos;

	CGameObject* m_pWaveText;
	CGameObject* m_pStart_End;

	float m_fStartTime;
	float m_fStartSpeed;

	float m_fEndTime;
	float m_fEndSpeed;

	int m_iWaveCount;


	vector<CGameObject*> m_vecStageStart;
	int m_iStageUiIndex;

};

