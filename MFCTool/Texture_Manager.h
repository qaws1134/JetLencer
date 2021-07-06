#pragma once
class CTexture; 
class CTexture_Manager
{
	DECLARE_SINGLETON(CTexture_Manager)
public:
	enum TEX_ID {SINGLE_TEX, MULTI_TEX};
private:
	CTexture_Manager();
	~CTexture_Manager();
public:
	HRESULT Insert_Texture_Manager(TEX_ID eID,
		const wstring & wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring & wstrStateKey = L"",
		const DWORD dwCount = 0);

	 const TEXINFO * Get_TexInfo(const wstring& wstrObjectKey,
		 const wstring & wstrStateKey = L"",
		 const DWORD dwIndex = 0);

	 void Release_Texture_Manager() ;
private:
	//ObjectKey
	map<wstring, CTexture*> m_mapTexture; 
};

