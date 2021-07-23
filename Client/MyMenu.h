#pragma once
#include "Scene.h"
class CMyMenu :
	public CScene
{
private:
	explicit CMyMenu();
public:
	virtual ~CMyMenu();

	static CScene* Create();
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
	void SpawnCloude();
	void Key_Check();
private:
	vector<CGameObject*> m_vecCloude;	//�޴� ����Ʈ 
	vector<_vec3> m_vecMenuIndex;	//�޴� ����Ʈ 
	vector<CGameObject*> m_vecUi;

	int			m_iMaxCloude;
	float		m_fIndexYOffset;
	
	float		m_fActionTime;
	float		m_fActionSpeed;
	
	bool		m_bAction;
	int			m_iActionIdx;
	int			m_iButtonIdx;

	bool		m_bDead;

};

