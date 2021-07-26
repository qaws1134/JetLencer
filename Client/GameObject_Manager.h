#pragma once
class CGameObject; 
class CCollider;
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	void Add_GameObject_Manager(OBJID::ID eID, CGameObject* pObject );
	void Update_GameObject_Manager(); 
	void Render_GameObject_Manager(); 
	void Release_GameObject_Manager(); 

	void DeleteID_GameObject_Manager(OBJID::ID eID);

public :
	CGameObject* Get_Mouse() { if (m_listGameObject[OBJID::MOUSE].empty())return nullptr; return m_listGameObject[OBJID::MOUSE].front(); }
	CGameObject* Get_Player() { if (m_listGameObject[OBJID::PLAYER].empty())return nullptr; return m_listGameObject[OBJID::PLAYER].front();  }
	CGameObject* Get_Target(CGameObject* _pObj, OBJID::ID _eID) const;

	bool Object_NullCheck(OBJID::ID _eID);
	void Set_AllTrueMode(bool _bTrueMod) { m_bTrueMod = _bTrueMod; }
	void Set_GameEnd() { m_bEnd = true; }

	bool Get_GameEnd() { return m_bEnd; }
	void Clear_LoadOutList() { m_listLoadOutSelect.swap(list<LOADOUT::INDEX>()); m_listLoadOutSelect.clear(); }
	void Set_LoadOutData(LOADOUT::INDEX _eIndex) { m_listLoadOutSelect.emplace_back(_eIndex); }
	list<LOADOUT::INDEX> Get_LoadOutData() { return m_listLoadOutSelect; }

private:
	list<CGameObject*> m_listGameObject[OBJID::END]; 
	list<CGameObject*> m_listGameObjectRender[RENDERID::END];
	list<CCollider*> m_listGameObjectCollider[COLLIDER::END];
	
	list<LOADOUT::INDEX>m_listLoadOutSelect;

	bool m_bEnd;
	bool m_bColRender;
	bool m_bTrueMod;
};

