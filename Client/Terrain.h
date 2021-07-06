#pragma once
#include "GameObject.h"
class CTerrain : public CGameObject
{
private:
	CTerrain();
public:
	~CTerrain();
public:
	void Set_TileInfo(TILE* pTile)
	{
		m_vecTile.emplace_back(pTile); 
	}
	vector<TILE*>& Get_VecTile() { return m_vecTile; }
private:
	void LoadTileData(const wstring& wstrFilePath);
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	static CGameObject* Create(); 

private:
	vector<TILE*> m_vecTile; 
};

