#pragma once
class CScroll_Manager
{
public:
	enum ID {SCROLL_X, SCROLL_Y};
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	static const D3DXVECTOR3& Get_Scroll() { return m_vScroll; }
	static void Set_Scroll(const D3DXVECTOR3& vScroll ); 
	static void Scroll_Lock();
	static void	Position_Lock();
	static bool Get_ScrollLock(SCROLL::ID _ePosition) { return m_bLock[_ePosition]; }
private:
	static D3DXVECTOR3 m_vScroll; 
	static bool			m_bLock[SCROLL::END];


};

