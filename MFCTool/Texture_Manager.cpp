#include "stdafx.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"
IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_Texture_Manager(); 
}

HRESULT CTexture_Manager::Insert_Texture_Manager(TEX_ID eID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD dwCount)
{
	map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(wstrObjectKey);
	CTexture* pTexture = nullptr;
	if (m_mapTexture.end() == iter_find)
	{
		
		switch (eID)
		{
		case CTexture_Manager::SINGLE_TEX:
			pTexture = new CSingle_Texture; 
			break;
		case CTexture_Manager::MULTI_TEX:
			pTexture = new CMulti_Texture; 
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))
			goto ERR; 
		
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (MULTI_TEX == eID)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))
			goto ERR; 

	}
	return S_OK; 
ERR:
	Safe_Delete(pTexture); 
	ERR_MSG(L"이미지 삽입을 이미지관리하는 놈이 실패했댄다!!"); 
	return E_NOTIMPL;
}

const TEXINFO * CTexture_Manager::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD dwIndex)
{
	map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(wstrObjectKey); 
	if(m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->Get_TexInfo(wstrStateKey, dwIndex);
	//return m_mapTexture[wstrObjectKey]->Get_TexInfo(wstrStateKey, dwIndex); 
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (pair<const wstring, CTexture*>& rPair : m_mapTexture)
		Safe_Delete(rPair.second);

	m_mapTexture.clear(); 
}
