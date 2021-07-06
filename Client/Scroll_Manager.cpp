#include "stdafx.h"
#include "Scroll_Manager.h"

D3DXVECTOR3 CScroll_Manager::m_vScroll{};
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

