#include "stdafx.h"
#include "Gui.h"


CGui::CGui()
{
}


CGui::~CGui()
{
	Release_GameObject();
}

CGameObject * CGui::Create(const PLACEMENT * _pPlacement)
{
	CGameObject* pInstance = new CGui;
	pInstance->Set_Placement(_pPlacement);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CGui::Ready_GameObject()
{
	m_eRenderId = m_pPlacement->eRenderID;

	return S_OK;
}

int CGui::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CGui::Late_Update_GameObject()
{
}

void CGui::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pPlacement->wstrName);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;

	//ÃÊ±âÈ­
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixScaling(&matScale, m_pPlacement->m_tMatInfo.mat[MATID::SCALE].x, m_pPlacement->m_tMatInfo.mat[MATID::SCALE].y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_pPlacement->m_tMatInfo.mat[MATID::ROT].z));
	D3DXMatrixTranslation(&matTrans, m_pPlacement->m_tMatInfo.mat[MATID::TRANS].x, m_pPlacement->m_tMatInfo.mat[MATID::TRANS].y, 0.f);
	matWorld = matScale *matRotZ* matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGui::Release_GameObject()
{
}

void CGui::State_Change()
{
}
