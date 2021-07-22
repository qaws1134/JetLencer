#pragma once
#include "SerpentObject.h"
class CSerpentHead :
	public CSerpentObject
{

private:
	explicit CSerpentHead();
public:
	virtual ~CSerpentHead();

public :
	static CGameObject* Create(const OBJECTINFO * _tObjectInfo);

	virtual HRESULT Ready_GameObject() override;
	virtual void Ai_State() override;
	virtual void State_Change() override;
	virtual void Select_Armor() override;


	bool m_bStart;



};

