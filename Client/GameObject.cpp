#include "stdafx.h"
#include "GameObject.h"
#include "Collider.h"

CGameObject::CGameObject()
	:m_pObjectInfo(nullptr)
	, m_pTarget(nullptr)
	, m_fAngleSpeed(0.f)
	, m_fAngle_per_Frame(0.f)
	, m_fAngleOffset(0.f)
	, m_fAngle(0.f)
	, m_fSpeed(0.f)
	, m_fMaxSpeed(0.f)
	, m_fAccel(0.f)
	, m_fRegistPower(0.f)
	, m_fCenterX(0.f)
	, m_fCenterY(0.f)
	, m_bDead(false)
	, m_bCenter(false)
	, m_bDeadEffect(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
	m_tColor = {255,255,255,255};
}


CGameObject::~CGameObject()
{
	for (auto& iter : m_vecCollider)
		iter->Set_Dead(true);
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



HRESULT CGameObject::Set_Texture()
{
	m_pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_tFrame.wstrObjKey,m_tFrame.wstrStateKey, (DWORD)m_tFrame.fStartFrame);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	return S_OK;
}

HRESULT CGameObject::Set_Texture(const wstring & wstrObjectKey,const wstring& wstrStateKey)
{
	m_pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(wstrObjectKey, wstrStateKey, (DWORD)m_tFrame.fStartFrame);
	NULL_CHECK_RETURN(m_pTexInfo, E_FAIL);
	return S_OK;
}
void CGameObject::Set_CenterPos(_vec3 vPos)
{
	m_fCenterX = vPos.x -float (WINCX >>1);
	m_fCenterY = vPos.y - float(WINCY >> 1);
	m_bCenter = true;

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
	//m_vTarget_Dir = m_pTarget->m_tInfo.vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_vTarget_Dir, &m_vTarget_Dir);

	m_tInfo.vDir = { cosf(D3DXToRadian(m_fAngle)),-sinf(D3DXToRadian(m_fAngle)),0.f };

	float fCos = D3DXVec3Dot(&m_vTarget_Dir, &m_tInfo.vDir);
	if (fCos > 0.99)
		return;
	float fAngle = D3DXToDegree(acosf(fCos));
	_vec3 vCross = {};
	D3DXVec3Cross(&vCross, &m_vTarget_Dir, &m_tInfo.vDir);
	if (vCross.z > 0)
		m_fAngle += m_fAngleSpeed*fTime;
	else if(vCross.z<0)
		m_fAngle -= m_fAngleSpeed*fTime;


	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
	if (m_fAngle < 0.f)
		m_fAngle = 360.f;
}

