#pragma once
class CTime_Manager
{
	DECLARE_SINGLETON(CTime_Manager)
public:
	explicit CTime_Manager();
	virtual ~CTime_Manager();
public:
	float Get_DeltaTime() { return m_fDeltaTime;  }
public:
	void Ready_Time_Manager(); 
	void Update_Time_Manager(); 

private:
	LARGE_INTEGER m_CpuTick; 
	LARGE_INTEGER m_BeginTime; 
	LARGE_INTEGER m_EndTime;

	float m_fDeltaTime; 

};

