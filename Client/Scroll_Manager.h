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
	static void Shake(float fPower,float fShakeSpeed);
	static bool Get_Shake() { return m_bShake; }
	static _vec3 Get_ShakeDir() { return m_vShakeDir; }

private:
	static D3DXVECTOR3 m_vScroll; 
	static bool			m_bLock[SCROLL::END];
	static bool			m_bShake;
	static bool			m_bReverse;
	static float		m_fPower;
	static float		m_fShakeTime;
	static float		m_fShakeSpeed;
	static _vec3		m_vShakeDir;


};

