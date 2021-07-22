#pragma once
#include "Jet.h"
class CSerpent :
	public CJet
{
private:
	explicit CSerpent();
public:
	virtual ~CSerpent();

public :
	static CGameObject* Create(_vec3 vPos);

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void State_Change() override;


	void DeadEffect();
	void Hp_Stage();
	void Select_Page();
	void Ai_State();
	void Move();
	void Init_Target();
	void Init_Pos();
	void Init_Hp();


	bool PatternTime();
	bool Hp_RedTime();

	vector<CGameObject*> Get_SerpentObj() { return m_vecSerpent; }


private:
	const OBJECTINFO* pHead;
	const OBJECTINFO* pBody;
	const OBJECTINFO* pTail;


	vector<CGameObject*> m_vecSerpent;

	SERPENT::PATTERN m_eSelected_Pattern;
	SERPENT::PATTERN m_eSerpentPagePattern;
	SERPENT::PATTERN m_eTailPattern;






	int m_iMaxHp;
	int m_iPreHp;
	int m_iMaxBody;
	int m_iHpOffset;


	bool m_bStart;
	bool m_bPattern_Start;
	bool m_bPage_Start;
	bool m_bCircle_Rager;

	bool m_bGravity;
	float m_fPatternTime;
	float m_fPatternEnd;

	float m_fHp_RedTime;
	float m_fHp_RedSpeed;

	float m_RandfX;
	float m_RandfY;

	int m_iCircleIndex;
	int	m_iCircleEndIdx;

	_vec3 m_vEnd_Dir;

	CGameObject* m_pBoss_Hp;
	CGameObject* m_pBoss_Hp_Plate;
	CGameObject* m_pBoss_Hp_Red;

	CGameObject* m_pChargeBeam;



};

