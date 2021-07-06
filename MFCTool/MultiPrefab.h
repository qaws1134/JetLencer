#pragma once

class CMultiPrefab
{
public:
	CMultiPrefab();
	~CMultiPrefab();
public:
	HRESULT Ready_Multi();
	void Render_Multi();
	void Release_Multi();
public:
	void MousePicking(const D3DXVECTOR3& vMousePos);
	
};


