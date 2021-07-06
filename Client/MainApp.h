#pragma once
class CGameObject; 
class CMainApp final 
{
	// abstract - 추상 - 클래스를 추상클래스로 만들어주는 키워드. 
	// 원래 추상 클래스는 순수 가상함수가 단 하나라도 있을 경우 만들어졌으나
	// abstract라는 키워드가 생기고 나서는 순수 가상함수가 없어도 추상 클래스화 시킬수 있다. 
	// 아주 . 협업하기 좋은 키워드일세 ? 안그래유 ? 
	// final - 이..이보게 의사양반... 그게 무슨말이오... - 야인시대 - feat. 심영. 
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

