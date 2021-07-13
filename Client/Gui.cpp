#include "stdafx.h"
#include "Gui.h"
#include "Prefab_Manager.h"

CGui::CGui() : m_wstrText(L"")
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

CGameObject * CGui::Create( _vec3 vPos,wstring _wstrText)
{
	CGameObject* pInstance = new CGui;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	pInstance->Set_Pos(vPos);
	static_cast<CGui*>(pInstance)->Set_Text(_wstrText);
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
	m_eRenderId =RENDERID::UI;
	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;

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
	FAILED_CHECK_RETURN(Set_Texture(), );
}
void CGui::WriteMatrix()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

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
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));
}

void CGui::Render_GameObject()
{
	WriteMatrix();
	TCHAR sztext[50] = L"";
	swprintf_s(sztext, m_wstrText.c_str(), m_fPoint);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), sztext, lstrlen(sztext), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

}

void CGui::Release_GameObject()
{
}

void CGui::State_Change()
{
}
