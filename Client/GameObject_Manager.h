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
public :
	CGameObject* Get_Mouse() { if (m_listGameObject[OBJID::MOUSE].empty())return nullptr; return m_listGameObject[OBJID::MOUSE].front(); }
	CGameObject* Get_Player() { if (m_listGameObject[OBJID::PLAYER].empty())return nullptr; return m_listGameObject[OBJID::PLAYER].front();  }
	CGameObject* Get_Target(CGameObject* _pObj, OBJID::ID _eID) const;
	void Set_AllTrueMode(bool _bTrueMod) { m_bTrueMod = _bTrueMod; }

private:
	list<CGameObject*> m_listGameObject[OBJID::END]; 
	list<CGameObject*> m_listGameObjectRender[RENDERID::END];
	list<CCollider*> m_listGameObjectCollider[COLLIDER::END];


	bool m_bColRender;
	bool m_bTrueMod;
};

