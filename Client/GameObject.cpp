#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	:m_pObjectInfo(nullptr)
	, m_pTarget(nullptr)
	, m_fAngleSpeed(0.f)
	, m_fSpeed(0.f)
	, m_fAccel(0.f)
	,m_fRegistPower(0.f)
	,m_fCenterX(0.f)
	,m_fCenterY(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
}


CGameObject::~CGameObject()
{
}


void CGameObject::WriteMatrix()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x,m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x+ vScroll.x,m_tInfo.vPos.y+ vScroll.y, 0.f);
	matWorld = matScale *matRotZ* matTrans;
 	float fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(m_pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	if((0==m_fCenterX|| 0==m_fCenterY))
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CGameObject::Angle_Change()
{
}

HRESULT CGameObject::Set_Texture()
{
	m_pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_tFrame.wstrObjKey,m_tFrame.wstrStateKey,m_tFrame.fStartFrame);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	return S_OK;
}

HRESULT CGameObject::Set_Texture(const wstring & wstrObjectKey,const wstring& wstrStateKey)
{
	m_pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(wstrObjectKey, wstrStateKey,m_tFrame.fStartFrame);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	return S_OK;
}
void CGameObject::Set_CenterPos(_vec3 vPos)
{
	m_fCenterX = vPos.x -float (WINCX >>1);
	m_fCenterY = vPos.y - float(WINCY >> 1);

}
HRESULT CGameObject::Set_Texture(const wstring & wstrObjectKey)
{
	m_pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(wstrObjectKey);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	return S_OK;
}

void CGameObject::TargetAngle_Check()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vDir = m_pTarget->m_tInfo.vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)),-sinf(D3DXToRadian(m_fAngle)),0.f };

	float fCos = D3DXVec3Dot(&vDir, &m_tInfo.vDir);
	if (fCos > 0.99)
		return;
	float fAngle = D3DXToDegree(acosf(fCos));
	_vec3 vCross = {};
	D3DXVec3Cross(&vCross, &vDir, &m_tInfo.vDir);
	if (vCross.z > 0)
		m_fAngle += m_fAngleSpeed*fTime;
	else if(vCross.z<0)
		m_fAngle -= m_fAngleSpeed*fTime;


	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
	if (m_fAngle < 0.f)
		m_fAngle = 360.f;
}

