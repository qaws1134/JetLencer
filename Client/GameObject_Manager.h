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
	CGameObject* Get_Mouse() { return m_listGameObject[OBJID::MOUSE].front(); }
	CGameObject* Get_Player() { if (!m_listGameObject[OBJID::PLAYER].empty())return m_listGameObject[OBJID::PLAYER].front(); return nullptr; }

private:
	list<CGameObject*> m_listGameObject[OBJID::END]; 
	list<CGameObject*> m_listGameObjectRender[RENDERID::END];
	list<CCollider*> m_listGameObjectCollider[COLLIDER::END];


	bool m_bColRender;
};

