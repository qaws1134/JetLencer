#pragma once
#include "Scene.h"
class CLoadOut :
	public CScene
{
private:
	explicit CLoadOut();
public:
	virtual ~CLoadOut();

	static CScene* Create();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
	
	
	
public :
	void Key_Check();
	void Show_Info();
	void Select_LoadOut(int _iIdx);
private:

	//아이콘 관리
	vector<CGameObject*> m_vecLoadOutFrame;
	vector<CGameObject*> m_vecLoadOutIcon;
	vector<CGameObject*> m_vecLoadOutBackplate;
	vector<CGameObject*> m_vecLoadOutInfo;
	//아이콘 인덱스
	vector<_vec3> m_vecFrameIndex;
	//외형 정보
	vector<CGameObject*> m_vecUiFrame;

	//아이콘을 가리킬 포인터
	CGameObject* m_pPointer;

	//플레이어에 넘겨줄 벡터
	vector<CGameObject*>m_vecLoadOutEquip;

	CGameObject* m_pBackGround;

	//저장 어디에?
	//매니져 오비제 메니져
	//플레이어 무기랑 모듈
	

	bool m_bChangeIndex;
	int m_iMaxWeaponFrame;
	int m_iTextLineInc;
	int m_iPointIndex;
	bool m_bDead;
};

