#include "stdafx.h"
#include "Terrain.h"
#include "MFCToolView.h"

CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Terrain()
{
	return S_OK;
}

void CTerrain::Render_Terrain()
{


}

void CTerrain::MiniRender_Terrain()
{


}

void CTerrain::Release_Terrain()
{

}

void CTerrain::TilePicking_Terrain(const D3DXVECTOR3 & vMousePos, const BYTE & byDrawID, const BYTE & byOption)
{

}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vMousePos)
{
	return 0;
}

bool CTerrain::IsPicking_Terrain(const D3DXVECTOR3 & vMousePos, int iIndex)
{
	return false;
}
