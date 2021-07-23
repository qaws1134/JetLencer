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

	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
	
	
	
public :
	void Key_Check();
	void Show_Info();
	void Select_LoadOut(int _iIdx);
private:

	//������ ����
	vector<CGameObject*> m_vecLoadOutFrame;
	vector<CGameObject*> m_vecLoadOutIcon;
	vector<CGameObject*> m_vecLoadOutBackplate;
	vector<CGameObject*> m_vecLoadOutInfo;
	//������ �ε���
	vector<_vec3> m_vecFrameIndex;
	//���� ����
	vector<CGameObject*> m_vecUiFrame;

	//�������� ����ų ������
	CGameObject* m_pPointer;

	//�÷��̾ �Ѱ��� ����
	vector<CGameObject*>m_vecLoadOutEquip;

	CGameObject* m_pBackGround;

	//���� ���?
	//�Ŵ��� ������ �޴���
	//�÷��̾� ����� ���
	

	bool m_bChangeIndex;
	int m_iMaxWeaponFrame;
	int m_iTextLineInc;
	int m_iPointIndex;
	bool m_bDead;
};

