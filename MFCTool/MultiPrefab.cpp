#include "stdafx.h"
#include "MultiPrefab.h"


CMultiPrefab::CMultiPrefab()
{
}


CMultiPrefab::~CMultiPrefab()
{
}

HRESULT CMultiPrefab::Ready_Multi()
{
	//스크롤로 오른쪽으로 wincx>>1만큼 이동
	//아래로 wincy>>1 만큼 이동
	return S_OK;
}

void CMultiPrefab::Render_Multi()
{
}

void CMultiPrefab::Release_Multi()
{
}

void CMultiPrefab::MousePicking(const D3DXVECTOR3 & vMousePos)
{
}
