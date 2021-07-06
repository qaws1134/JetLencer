#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();
public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const DWORD dwCount = 0 )PURE; 
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"",
		const DWORD dwIndex = 0)PURE; 
	virtual void Release_Texture() PURE; 

	// ObjectKey, StateKey
	//이건 멀티 텍스쳐일 경우야 ㅋㅋ 
	//map<wstring, CMulti_Texture(map<wstring, vector<TEXINFO*>>)> 하하하하하핳 
};

