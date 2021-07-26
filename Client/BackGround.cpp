#include "stdafx.h"
#include "BackGround.h"
#include "GameObject_Manager.h"
#include "Spawn_Manager.h"
#include "Player.h"

CBackGround::CBackGround() : m_fSplashTime(0.f), m_iAlpha(0),m_bBossLaser(false)
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
	vMaxScale = m_tInfo.vSize + m_tInfo.vSize*0.2f;
	vMinScale = m_tInfo.vSize;


	if (m_eRenderId == RENDERID::TRUE_FACT)
		m_tColor = { 0,255,255,255 };
	return S_OK;
}

int CBackGround::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();



	return OBJ_NOEVENT;
}

void CBackGround::Late_Update_GameObject()
{
	State_Change();
	Set_Texture(m_pPlacement->wstrName);
}
void CBackGround::WriteMatrix()
{
	if (!m_pTexInfo)
		return;

	_vec3 vOffsetScroll = CScroll_Manager::Get_Scroll() - m_tInfo.vDir;

	if (CScroll_Manager::Get_Shake())
	{
		vOffsetScroll -= CScroll_Manager::Get_ShakeDir()*1.2f;
	}
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
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(m_fCenterX, m_fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_tColor.iAlpha, m_tColor.iRed, m_tColor.iGreen, m_tColor.iBlue));

}
void CBackGround::Render_GameObject()
{
	if (m_bTrueMod || m_bAllTrueMod)
	{
		m_tColor.iRed = 0;
		m_tColor.iGreen = 0;
		m_tColor.iBlue = 0;
	}
	else
	{
		m_tColor.iRed = 255;
		m_tColor.iGreen = 255;
		m_tColor.iBlue = 255;
	}

	if (m_eRenderId != RENDERID::TRUE_FACT)
	{
		if (m_bTrueMod || m_bAllTrueMod)
		{
			m_tColor.iRed = 0;
			m_tColor.iGreen = 0;
			m_tColor.iBlue = 0;
		}
		else
		{
			m_tColor.iRed = 255;
			m_tColor.iGreen = 255;
			m_tColor.iBlue = 255;
		}
	}
	if (m_eRenderId == RENDERID::MOVE_BACKGROUND1)
	{
		m_tInfo.vDir = _vec3{ CScroll_Manager::Get_Scroll().x*0.9f,CScroll_Manager::Get_Scroll().y,0.f };

		if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Zoom())
		{
			if (D3DXVec3Length(&vMaxScale) > D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize += m_tInfo.vSize*0.003f;
			else
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Zoom(false);			
		}
		else
		{
			if (D3DXVec3Length(&vMinScale) < D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize -= vMinScale*0.001f;
		}
	}
	if (m_eRenderId == RENDERID::MOVE_BACKGROUND2)
	{
		m_tInfo.vDir = _vec3{CScroll_Manager::Get_Scroll().x*0.93f,CScroll_Manager::Get_Scroll().y,0.f };
		if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Zoom())
		{
			if (D3DXVec3Length(&vMaxScale) > D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize += m_tInfo.vSize*0.003f;
			else
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Zoom(false);
		}
		else
		{
			if (D3DXVec3Length(&vMinScale) < D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize -= vMinScale*0.001f;
		}
	}
	if (m_eRenderId == RENDERID::MOVE_BACKGROUND3)
	{
		m_tInfo.vDir = _vec3{CScroll_Manager::Get_Scroll().x*0.95f,CScroll_Manager::Get_Scroll().y,0.f };
		if (static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Zoom())
		{
			if (D3DXVec3Length(&vMaxScale) > D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize += m_tInfo.vSize*0.003f;
			else
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Zoom(false);
		}
		else
		{
			if (D3DXVec3Length(&vMinScale) < D3DXVec3Length(&m_tInfo.vSize))
				m_tInfo.vSize -= vMinScale*0.001f;
		}
	}
	if (m_eRenderId == RENDERID::GROUND)
	{
		m_tInfo.vDir = _vec3{ CScroll_Manager::Get_Scroll().x,0.f,0.f };

		if (m_tInfo.vPos.y - float(m_pTexInfo->tImageInfo.Height)-150.f < m_pTarget->Get_ObjInfo().vPos.y)
		{
			if (m_tInfo.vPos.y - float(m_pTexInfo->tImageInfo.Height)+200.f > m_pTarget->Get_ObjInfo().vPos.y)
			{
				float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
				m_fSplashTime += fTime;

				//땅과 플레이어의 y값 차
				float fYGap = (m_pTarget->Get_ObjInfo().vPos.y) - (m_tInfo.vPos.y - float(m_pTexInfo->tImageInfo.Height) - 100.f);
				float fYNormalGap = fYGap* 0.01f;

				//최대 크기
				if (fYNormalGap > 1.5f)
					fYNormalGap = 1.5f;

				if (m_fSplashTime > 0.015f)
				{
					CSpawn_Manager::Spawn(EFFECT::GROUND_WATERSPLASH_FAST, _vec3(m_pTarget->Get_ObjInfo().vPos.x - m_pTarget->Get_Velocity().x*fTime, m_tInfo.vPos.y - float(m_pTexInfo->tImageInfo.Height)*0.5f, 0.f), false, fYNormalGap);
					m_fSplashTime = 0.f;
				}
			}
		}

	}
	if (m_eRenderId == RENDERID::BACKGROUND)
	{

		//사다리꼴 모양으로 만들고 y 스케일 조정 

		m_tInfo.vDir = CScroll_Manager::Get_Scroll();
	}
	if (m_eRenderId == RENDERID::TRUE_FACT)
	{
		if (m_bTrueMod || m_bAllTrueMod)
		{
			m_tColor = { 255,255,255,255 };
		}
		else
		{
			m_tColor = { 0,255,255,255 };
		}
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

