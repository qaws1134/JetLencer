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
void CBackGround::WriteMatrix()
{
	//D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	_vec3 vOffsetScroll = CScroll_Manager::Get_Scroll() - m_tInfo.vDir;
	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vOffsetScroll.x, m_tInfo.vPos.y + vOffsetScroll.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;
	float fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	if ((0 == m_fCenterX || 0 == m_fCenterY))
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}
void CBackGround::Render_GameObject()
{
	if (m_eRenderId == RENDERID::MOVE_BACKGROUND1)
	{
		m_tInfo.vDir = _vec3{ CScroll_Manager::Get_Scroll().x*0.9f,CScroll_Manager::Get_Scroll().y,0.f };
	}
	else if (m_eRenderId == RENDERID::MOVE_BACKGROUND2)
	{
		m_tInfo.vDir = _vec3{CScroll_Manager::Get_Scroll().x*0.8f,CScroll_Manager::Get_Scroll().y,0.f };
	}
	else if (m_eRenderId == RENDERID::MOVE_BACKGROUND3)
	{
		m_tInfo.vDir = _vec3{CScroll_Manager::Get_Scroll().x*0.7f,CScroll_Manager::Get_Scroll().y,0.f };
		m_fCenterY = float(m_pTexInfo->tImageInfo.Height);
		m_fCenterX = float(m_pTexInfo->tImageInfo.Width>>1);

	}
	else if(m_eRenderId == RENDERID::GROUND)
	{
		m_tInfo.vDir = _vec3{ CScroll_Manager::Get_Scroll().x,0.f,0.f };
	}
	else if (m_eRenderId == RENDERID::BACKGROUND)
	{
		//사다리꼴 모양으로 만들고 y 스케일 조정 
		m_tInfo.vDir = CScroll_Manager::Get_Scroll();
	}
	WriteMatrix();
}

void CBackGround::State_Change()
{
}
void CBackGround::Release_GameObject()
{
}

