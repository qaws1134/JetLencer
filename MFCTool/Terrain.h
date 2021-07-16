#pragma once
class CMFCToolView; 
class CTerrain
{
public:
	CTerrain();
	~CTerrain();
public:
	void Set_TileInfo(TILE* pTile)
	{
		m_vecTile.emplace_back(pTile); 
	}
	vector<TILE*>& Get_VecTile() { return m_vecTile; }
public:
	void Set_View(CMFCToolView* pView) { m_pView = pView;  }
public:
	HRESULT Ready_Terrain();
	void Render_Terrain(); 
	void Release_Terrain(); 
public:
	void TilePicking_Terrain(const D3DXVECTOR3& vMousePos, const BYTE& byDrawID, const BYTE& byOption = 0 ); 
private:
	int Get_TileIndex(const D3DXVECTOR3& vMousePos);
	bool IsPicking_Terrain(const D3DXVECTOR3& vMousePos, int iIndex );


private:
	vector<TILE*> m_vecTile; 
	CMFCToolView* m_pView; 
};

