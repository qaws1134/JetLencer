#include "stdafx.h"
#include "Frame_Manager.h"


CFrame_Manager::CFrame_Manager()
	:m_fDeltaTime(0.f)
	, m_iFPS(0)
, m_szFPS(L"")
, m_fFPSTime(0.f)
{
}


CFrame_Manager::~CFrame_Manager()
{
}

void CFrame_Manager::Ready_Frame_Manager(float fFPS)
{
	m_fSPF = 1.f / fFPS; 
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_BeginTime);
	QueryPerformanceCounter(&m_EndTime);
}

bool CFrame_Manager::FrameLock()
{
	QueryPerformanceCounter(&m_EndTime); 
	m_fDeltaTime += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart; 
	m_BeginTime = m_EndTime; 
	if (m_fDeltaTime >= m_fSPF)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_fDeltaTime = 0.f; 
		++m_iFPS; 
		return true; 
	}
	return false;
}

void CFrame_Manager::Render_Frame_Manager()
{
	m_fFPSTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (1.f <= m_fFPSTime)
	{
		m_fFPSTime = 0.f; 
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0; 
	}
	SetWindowText(g_hWND, m_szFPS); 
}
