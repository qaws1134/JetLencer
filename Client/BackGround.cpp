#include "stdafx.h"
#include "BackGround.h"


CBackGround::CBackGround()
{
}


CBackGround::~CBackGround()
{
	Release_GameObject();
}

CGameObject * CBackGround::Create(const PLACEMENT * _pPlacement)
{
	CGameObject* pInstance = new CBackGround;
	pInstance->Set_Placement(_pPlacement);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBackGround::Ready_GameObject()
{
	m_eRenderId = m_pPlacement->eRenderID;
	m_tInfo.vPos = m_pPlacement->m_tMatInfo.mat[MATID::TRANS];
	m_tInfo.vSize = m_pPlacement->m_tMatInfo.mat[MATID::SCALE];
	m_fAngle = m_pPlacement->m_tMatInfo.mat[MATID::ROT].z;

	return S_OK;
}

int CBackGround::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CBackGround::Late_Update_GameObject()
{
	Set_Texture(m_pPlacement->wstrName);
}

void CBackGround::Render_GameObject()
{
	//const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_pPlacement->wstrName);
	//if (nullptr == pTexInfo)
	//	return;

	WriteMatrix();
	//D3DXMATRIX matScale, matTrans, matRotZ, matWorld;

	//D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	////ÃÊ±âÈ­
	//D3DXMatrixIdentity(&matScale);
	//D3DXMatrixIdentity(&matTrans);
	//D3DXMatrixIdentity(&matRotZ);
	//float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	//float fCenterY = float(pTexInfo->tImageInfo.Height >> 1); 
	//D3DXMatrixScaling(&matScale, m_pPlacement->m_tMatInfo.mat[MATID::SCALE].x, m_pPlacement->m_tMatInfo.mat[MATID::SCALE].y, 0.f);
	//D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_pPlacement->m_tMatInfo.mat[MATID::ROT].z));
	//D3DXMatrixTranslation(&matTrans, m_pPlacement->m_tMatInfo.mat[MATID::TRANS].x + vScroll.x, m_pPlacement->m_tMatInfo.mat[MATID::TRANS].y + vScroll.y, 0.f);
	//matWorld = matScale *matRotZ* matTrans;
	//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBackGround::Release_GameObject()
{
}

void CBackGround::State_Change()
{
}
