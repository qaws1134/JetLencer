#pragma once

#define OBJ_DEAD 1 
#define OBJ_NOEVENT 0 


#ifdef _AFX
#define ERR_MSG(msg)		AfxMessageBox(msg)
#else 
#define ERR_MSG(msg)		MessageBox(nullptr,msg, L"System_Error", MB_OK)
#endif // _AFX

#define NULL_CHECK_RETURN(_obj, _return)				\
if(nullptr == _obj) return _return;

#define FAILED_CHECK_RETURN(_hr, _return)				\
if(_hr < 0) return _return;								



#define DECLARE_SINGLETON(ClassName)					\
public:													\
static ClassName* Get_Instance()						\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
static void Destroy_Instance()							\
{														\
	if (m_pInstance)									\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}														\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;