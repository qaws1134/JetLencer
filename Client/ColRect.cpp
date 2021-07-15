#include "stdafx.h"
#include "ColRect.h"


CColRect::CColRect()
{
}


CColRect::~CColRect()
{
}

CCollider * CColRect::Create(CGameObject * pTarget, float _fWidth, float _fHeight,COLLIDER::ID _eColId)
{
	CCollider* pInstance = new CColRect;
	pInstance->Set_Target(pTarget);
	static_cast<CColRect*>(pInstance)->Set_ColRect(_fWidth, _fHeight);
	static_cast<CColRect*>(pInstance)->Set_ColId(_eColId);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

void CColRect::WriteMatrix()
{
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePos[5] = { { (float)m_ColRect.left, (float)m_ColRect.top}
	,{ (float)m_ColRect.right, (float)m_ColRect.top}
	,{ (float)m_ColRect.right, (float)m_ColRect.bottom}
	,{ (float)m_ColRect.left, (float)m_ColRect.bottom}
	,{ (float)m_ColRect.left, (float)m_ColRect.top } };

	CGraphic_Device::Get_Instance()->Get_Line()->SetWidth(3.0f);
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePos, 5, D3DCOLOR_ARGB(255, 255, 0, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);


}
