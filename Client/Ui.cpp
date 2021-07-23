#include "stdafx.h"
#include "Ui.h"
#include "Prefab_Manager.h"

CUi::CUi() :m_bRender(true), m_fOffset(0.f)
{
}


CUi::~CUi()
{
	Release_GameObject();
}




void CUi::Late_Update_GameObject()
{
	FAILED_CHECK_RETURN(Set_Texture(), );
	State_Change();
}

void CUi::Set_Frame(const ANIMATION * _tAnimationInfo)
{
	m_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	m_tFrame.fStartFrame = 0;
	m_tFrame.fMaxFrame = (float)m_pAnimation->iMax_Index;
}

void CUi::WriteMatrix()
{
	if (!m_pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x , m_tInfo.vPos.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;

	if (!m_bCenter)
	{
		float fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
		m_fCenterX = fCenterX;
		m_fCenterY = fCenterY;
	}
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(
		m_pTexInfo->pTexture, 
		nullptr, 
		&D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), 
		nullptr, 
		D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));
}

void CUi::Render_GameObject()
{
	if(m_bRender)
		WriteMatrix();
}

void CUi::Release_GameObject()
{
}
