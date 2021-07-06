#pragma once
class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device)
private:
	CGraphic_Device();
	~CGraphic_Device();
	void operator=(const CGraphic_Device& rhs) {} 
	CGraphic_Device(const CGraphic_Device& rhs) {}; 
public:
	LPD3DXLINE			Get_Line()		{ return m_pLine;	}
	LPD3DXSPRITE		Get_Sprite()	{ return m_pSprite;  }
	LPD3DXFONT			Get_Font()		{ return m_pFont;  }
	LPDIRECT3DDEVICE9	Get_Device()	{ return m_pDevice; }
public:
	HRESULT Ready_Graphic_Device(); 
	void Release_Graphic_Device(); 

public:
	void Render_Begin(); 
	void Render_End(HWND hWnd = nullptr); 
private:
	// 장치를 초기화 하는 일련의 과정 
	//1. 장치의 수준을 조사할 컴객체라는 놈을 생성해야됨. 
	//2. 장치의 수준을 조사하고 
	//3. 그 수준을 토대로 장치를 제어하는 컴객체를 생성할 것이다. 
	
	//compenent oBject model 의 약자 . 
	//레고 구멍 4개짜리 2개 짜리 꺾인거 등등. 하나의 완성품을 만들어내잖아 그지 ? 
	//  다 이해됐어요 ?? 
	//
	LPDIRECT3D9			m_pSDK; // 장치(여기서는 그래픽카드)의 수준을 조사해줄 컴객체. 
	LPDIRECT3DDEVICE9	m_pDevice; // 애가 실질적으로 장치를 제어할 컴객체야. 
	LPD3DXSPRITE		m_pSprite; 
	LPD3DXFONT			m_pFont; 
	LPD3DXLINE			m_pLine; 
};

