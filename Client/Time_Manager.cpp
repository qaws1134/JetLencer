#include "stdafx.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Player.h"
IMPLEMENT_SINGLETON(CTime_Manager)
CTime_Manager::CTime_Manager() :m_bStop(false), m_fPreDeltaTime(0.f), m_fStoplimit(0.f), m_fStopTime(0.f)
{
}


CTime_Manager::~CTime_Manager()
{
}

void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceFrequency(&m_CpuTick); 
	QueryPerformanceCounter(&m_BeginTime);
	QueryPerformanceCounter(&m_EndTime); 

}

void CTime_Manager::Update_Time_Manager()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	if (!m_bStop) 
	{
		m_fDeltaTime = float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;
		m_fPreDeltaTime = m_fDeltaTime;
	}
	else
	{
		m_fDeltaTime = 0;
		m_fStopTime += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;
		
		if (m_fStoplimit > m_fStopTime)
		{
			
			if (PLAYER::ROLL == static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_State())
			{
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_SuperEvade(true);
				m_fDeltaTime = m_fPreDeltaTime;
				m_fStopTime = 0.f;
				m_bStop = false;
				return;
			}
		}
		else if (m_fStoplimit<m_fStopTime)
		{
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_State(PLAYER::HIT);
			m_fDeltaTime = m_fPreDeltaTime;
			m_fStopTime = 0.f;
			m_bStop = false;
			return;
		}
	}
	m_BeginTime = m_EndTime;
}
