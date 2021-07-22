#pragma once
#include "SerpentObject.h"
class CSerpentTail :
	public CSerpentObject
{
private:
	explicit CSerpentTail();
public:
	virtual ~CSerpentTail();
public :
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void Ai_State() override;

	virtual void State_Change() override;


	int m_iAttackCount;
	bool m_bTriple;

	// CSerpentObject을(를) 통해 상속됨
	virtual void Select_Armor() override;

};

