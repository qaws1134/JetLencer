#pragma once
#include "SerpentObject.h"
class CSerpentBody:
	public CSerpentObject
{
private:
	explicit CSerpentBody();
public:
	virtual ~CSerpentBody();
public :
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo);


	virtual HRESULT Ready_GameObject() override;
	virtual void Ai_State() override;
	virtual void State_Change() override;



	vector<const ANIMATION*> m_vecBody_Gun;

	bool m_bAniEnd;

	// CSerpentObject을(를) 통해 상속됨
	virtual void Select_Armor() override;
};

