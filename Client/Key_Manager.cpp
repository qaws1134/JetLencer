#include "stdafx.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CKey_Manager)
CKey_Manager::CKey_Manager()
	:m_dwKey(0)
	,m_dwKeyDown(0)
	,m_dwKeyUp(0)
{
}


CKey_Manager::~CKey_Manager()
{
}

void CKey_Manager::Key_Update()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwKey |= KEY_O;
	if (GetAsyncKeyState('P')&0x8000)
		m_dwKey |= KEY_P;
}

bool CKey_Manager::Key_Up(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Down(DWORD dwKey)
{
	// 키가 눌렸을 때 동작하게끔 만들고 싶은거!!! 
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKey_Manager::Key_Pressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}
