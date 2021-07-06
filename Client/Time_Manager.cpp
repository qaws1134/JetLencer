#include "stdafx.h"
#include "Time_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)
CTime_Manager::CTime_Manager()
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
	m_fDeltaTime = float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart; 
	m_BeginTime = m_EndTime;
}
