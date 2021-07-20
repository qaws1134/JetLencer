#pragma once
//0000	0001
#define KEY_LBUTTON 0x00000001 
//0000	0010
#define KEY_RBUTTON 0x00000002
//0000	0100
#define KEY_UP		0x00000004
//0000	1000
#define KEY_DOWN	0x00000008
//0001	0000
#define KEY_LEFT	0x00000010
//0010	0000
#define KEY_RIGHT	0x00000020
//0100 0000
#define KEY_SPACE	0x00000040
//0100 0000
#define KEY_W		0x00000080

#define KEY_E		0x00000100

#define KEY_Q		0x00000200

#define KEY_O		0x00000400

#define KEY_P		0x00000800

#define KEY_L		0x00001000

#define KEY_NUM1	0x00002000

#define KEY_NUM2	0x00004000

#define KEY_NUM3	0x00008000

#define KEY_NUM4	0x00010000

class CKey_Manager
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager(); 
	~CKey_Manager(); 
public:
	void Key_Update();

	bool Key_Up(DWORD dwKey);

	bool Key_Down(DWORD dwKey);

	bool Key_Pressing(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;
};