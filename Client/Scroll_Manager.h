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

private:
	static D3DXVECTOR3 m_vScroll; 
};

