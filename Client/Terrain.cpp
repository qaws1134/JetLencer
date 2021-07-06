#include "stdafx.h"
#include "Terrain.h"

CTerrain::CTerrain()
{
}
CTerrain::~CTerrain()
{
	Release_GameObject();
}
void CTerrain::LoadTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwbyte = 0;
	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwbyte, nullptr);

		if (0 == dwbyte)
		{
			Safe_Delete(pTile);
			return;
		}
		m_vecTile.emplace_back(pTile);
	}
	CloseHandle(hFile);

}
HRESULT CTerrain::Ready_GameObject()
{
	//LoadTileData(L"../Data/TileData.dat");
	// 자 여기서 예외 처리 하셈 . 
	return S_OK;
}

int CTerrain::Update_GameObject()
{
	float fSpeed = 300.f * CTime_Manager::Get_Instance()->Get_DeltaTime();
	//if (CKey_Manager::Get_Instance()->Key_Up(KEY_LEFT))
	//	CScroll_Manager::Set_Scroll({ fSpeed, 0.f, 0.f });
	//if (CKey_Manager::Get_Instance()->Key_Down(KEY_RIGHT))
	//	CScroll_Manager::Set_Scroll({ -fSpeed, 0.f, 0.f });
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	CScroll_Manager::Set_Scroll({ 0.f,fSpeed,  0.f });
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	CScroll_Manager::Set_Scroll({ 0.f,-fSpeed,  0.f });


	//A* b* C스타!!! 
	return 0;
}

void CTerrain::Late_Update_GameObject()
{
}

void CTerrain::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans;
	int iSize = m_vecTile.size();
	TCHAR szIndex[32] = L"";
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	for (int i = 0; i < iSize; ++i)
	{
		swprintf_s(szIndex, L"%d", i);
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);

		if (nullptr == pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + vScroll.x, m_vecTile[i]->vPos.y + vScroll.y, 0.f);
		matScale *= matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matScale);

		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePos[2] = { { 100.f, 100.f },{ 200.f, 200.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePos, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

}

void CTerrain::Release_GameObject()
{
	for (TILE*& pTile : m_vecTile)
		Safe_Delete(pTile);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

CGameObject * CTerrain::Create()
{
	CGameObject* pInstance = new CTerrain;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}
