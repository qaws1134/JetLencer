#include "stdafx.h"
#include "GameObject.h"
#include "Collider.h"
#include "Spawn_Manager.h"
#include "Scroll_Manager.h"
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
	, m_fColorTime(0.f)
	, m_fColorSpeed(0.1f)
	, m_bTrueMod(false)
	, m_bAllTrueMod(false)
	, m_bColor(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
	m_tColor = {255,255,255,255};
	m_vecCollider.reserve(4);
}


CGameObject::~CGameObject()
{
	if (!m_vecCollider.empty())
	{
		for (auto& iter : m_vecCollider)
			iter->Set_Dead(true);
	}
	m_vecCollider.swap(vector<CCollider*>());
}


void CGameObject::WriteMatrix()
{
	if (!m_pTexInfo)
		return;
	if (m_bTrueMod|| m_bAllTrueMod)
	{
		m_tColor.iRed = 0;
		m_tColor.iGreen = 0;
		m_tColor.iBlue = 0;
	}
	else
	{
		if (!m_bColor)
		{
			m_tColor.iRed = 255;
			m_tColor.iGreen = 255;
			m_tColor.iBlue = 255;
		}
	}

	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();
	if (CScroll_Manager::Get_Shake())
	{
		vScroll += CScroll_Manager::Get_ShakeDir();
	}

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

CCollider * CGameObject::Get_ColVec(int i)
{
	if (m_vecCollider.empty())
		return nullptr; 
	vector<CCollider*>::iterator iter = m_vecCollider.begin(); 
	return iter[i];
}


void CGameObject::RandomEffect(EFFECT::TYPE _eEffType, int iNum,int iDis)
{
	for (int i = 0; i < rand() % iNum+1; i++)
	{
		float fRandomX = float(rand() % iDis);
		float fRandomY = float(rand() % iDis);
		if (rand() % 2 == 0)
			fRandomX *= -1.f;
		if (rand() % 2 == 0)
			fRandomY *= -1.f;
		_vec3 vRanPos = { m_tInfo.vPos.x + fRandomX,m_tInfo.vPos.y + fRandomY,0.f };
		CSpawn_Manager::Spawn(_eEffType, vRanPos, false);
	}
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
	m_vCross = {};
	D3DXVec3Cross(&m_vCross, &m_vTarget_Dir, &m_tInfo.vDir);

	if (m_vCross.z >= 0)
		m_fAngle += m_fAngleSpeed*fTime;
	else if(m_vCross.z<0)
		m_fAngle -= m_fAngleSpeed*fTime;

	if (m_fAngle > 360.f)
		m_fAngle = 0.f;
	if (m_fAngle < 0.f)
		m_fAngle = 360.f;

}

