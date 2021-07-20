#include "stdafx.h"
#include "Scroll_Manager.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
D3DXVECTOR3 CScroll_Manager::m_vScroll{};
bool CScroll_Manager::m_bLock[SCROLL::END];
bool CScroll_Manager::m_bShake= false;
bool CScroll_Manager::m_bReverse = false;

float CScroll_Manager::m_fShakeTime = 0.f;
float CScroll_Manager::m_fShakeSpeed = 0.f;
float CScroll_Manager::m_fPower = 0.f;
_vec3 CScroll_Manager::m_vShakeDir{};

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
	{
		m_vScroll.x = 0;
	}

	if (0 < m_vScroll.y)
		m_vScroll.y = 0;
	
	if (WINCX - Map_Width > m_vScroll.x)
	{
		m_vScroll.x = WINCX - Map_Width;
	}

	if (-Map_Height + Map_HeightOffset > m_vScroll.y)
		m_vScroll.y = -Map_Height + Map_HeightOffset;


	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fShakeTime += fTime;
	if (m_fShakeTime > m_fShakeSpeed)
	{
		m_bShake = false;
	}
	else
	{
		m_fPower *= 0.9f;

		float _fPowerX = m_fPower;
		float _fPowerY = m_fPower;

		if (rand() % 2 == 0)
			_fPowerX *= -1.f;
		if (rand() % 2 == 0)
			_fPowerY *= -1.f;
		m_vShakeDir = { _fPowerX,_fPowerY,0.f };
		
	}
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

void CScroll_Manager::Shake(float fPower, float fShakeSpeed)
{
	m_bShake = true;
	m_fShakeTime = 0.f;
	m_fShakeSpeed = fShakeSpeed;
	m_fPower = fPower;
}
