#include "stdafx.h"
#include "Single_Texture.h"


CSingle_Texture::CSingle_Texture()
	:m_tTexInfo({})
{
}


CSingle_Texture::~CSingle_Texture()
{
	Release_Texture(); 
}

HRESULT CSingle_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD dwCount /*= 0*/)
{
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tImageInfo)))
		goto ERR; 

	if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		m_tTexInfo.tImageInfo.Width, // 그림 가로
		m_tTexInfo.tImageInfo.Height, // 그림 세로 
		m_tTexInfo.tImageInfo.MipLevels, // 밉레벨 
		// 이미지 밉래벨은 있을수도 있고 없을 수도 있다. 
		0, 
		m_tTexInfo.tImageInfo.Format, 
		D3DPOOL_MANAGED, // 우리는 짬짜면을 쓰겠다. ㅇㅋ? 
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr, 
		&m_tTexInfo.pTexture)))
		goto ERR; 

	return S_OK;
ERR:
	ERR_MSG(L"그림한장 집어넣기 실패.");  
	return E_NOTIMPL;
}

const TEXINFO * CSingle_Texture::Get_TexInfo(const wstring & wstrStateKey /*= L""*/, const DWORD dwIndex /*= 0*/)
{
	return &m_tTexInfo;
}

void CSingle_Texture::Release_Texture()
{
	if (m_tTexInfo.pTexture)
		m_tTexInfo.pTexture->Release();
}
