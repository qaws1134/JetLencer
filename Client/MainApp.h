#pragma once
class CGameObject; 
class CMainApp final 
{
private:
	CMainApp();
public:
	~CMainApp();
public:
	HRESULT Ready_MainApp(); 
	void	Update_MainApp(); 
	void	Render_MainApp(); 
	void	Release_MainApp(); 
public:
	static CMainApp* Create();
private:

};

