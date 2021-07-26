#pragma once
#include "Scene.h"
class CStage2 :
	public CScene
{
private:
	explicit CStage2();
public:
	virtual ~CStage2();

	static CScene* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


public :
	void Set_StartStage() { m_bStart = true; }
	
private:

	float m_fSpawnTime;
	float m_fSpawnSpeed;

	float m_fWaveTime;
	float m_fWaveSpeed;

	bool m_bStart;
	bool m_bEnd;
	
	float m_fStartTime;
	float m_fStartSpeed;

	float m_fEndTime;
	float m_fEndSpeed;



	int m_iSpawnCount;
	int m_iWaveCount;
	list<wstring> m_listEnemy[3];
	vector<_vec3> m_vecSpawnPos;

	CGameObject* m_pWaveText;
	CGameObject* m_pStart_End;


	vector<CGameObject*> m_vecStageStart;
	int m_iStageUiIndex;

};

