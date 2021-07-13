#include "stdafx.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
D3DXVECTOR3 CScroll_Manager::m_vScroll{};
bool CScroll_Manager::m_bLock[SCROLL::END];

CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

void CScroll_Manager::Set_Scroll(const D3DXVECTOR3& vScroll)
{
	m_vScroll += vScroll; 
}

#define Map_HeightOffset 500
void CScroll_Manager::Scroll_Lock()
{
	Position_Lock();

	if (0 < m_vScroll.x)
		m_vScroll.x = 0;
	

	if (0 < m_vScroll.y)
		m_vScroll.y = 0;
	
	if (WINCX - Map_Width > m_vScroll.x)
		m_vScroll.x = WINCX - Map_Width;
	

	if (-Map_Height + Map_HeightOffset > m_vScroll.y)
		m_vScroll.y = -Map_Height + Map_HeightOffset;
	
}

void CScroll_Manager::Position_Lock()
{
	if (0 < m_vScroll.x)
		m_bLock[SCROLL::LEFT] = true;
	else
		m_bLock[SCROLL::LEFT] = false;

	if (0 < m_vScroll.y)
		m_bLock[SCROLL::TOP] = true;
	else
		m_bLock[SCROLL::TOP] = false;

	if (WINCX - Map_Width > m_vScroll.x)
		m_bLock[SCROLL::RIGHT] = true;
	else
		m_bLock[SCROLL::RIGHT] = false;

	if (Map_HeightOffset - Map_Height  > m_vScroll.y)
		m_bLock[SCROLL::BOTTOM] = true;
	else
		m_bLock[SCROLL::BOTTOM] = false;

}
