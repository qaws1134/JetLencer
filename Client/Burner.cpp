#include "stdafx.h"
#include "Burner.h"
#include "Prefab_Manager.h"

CBurner::CBurner()
	:m_ePreBurnerState(BURNER::END)
	, m_eBurnerState(BURNER::END)
	, m_bRender(true)
	, m_bLoop(false)

{
}


CBurner::~CBurner()
{
}

CGameObject * CBurner::Create()
{
	CGameObject* pInstance = new CBurner;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CBurner::Ready_GameObject()
{
	m_tFrame.wstrObjKey = L"Player";
	m_tFrame.wstrStateKey = L"brn";
	m_eRenderId = RENDERID::BURNER;
	m_tInfo.vSize = { 1.3f,1.3f,0.f };
	return S_OK;
}

int CBurner::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	State_Change();
	Frame_Change();

	return OBJ_NOEVENT;
}

void CBurner::Late_Update_GameObject()
{
	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CBurner::Render_GameObject()
{
	if (!m_pTexInfo)
		return;
	if (m_bRender)
		WriteMatrix();
}

void CBurner::Release_GameObject()
{
}
void CBurner::WriteMatrix()
{
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matRotZ, matWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRotZ);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale *matRotZ* matTrans;
	float fCenterX = float(m_pTexInfo->tImageInfo.Width );
	float fCenterY = float(m_pTexInfo->tImageInfo.Height >>1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(m_pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}
void CBurner::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bLoop)
		{
			m_tFrame.fStartFrame = 0;
		}
		else
 			m_eBurnerState = BURNER::AFTER;
	}
}
void CBurner::State_Change()
{
	if (m_ePreBurnerState != m_eBurnerState)
	{
		switch (m_eBurnerState)
		{
		case BURNER::IDLE:
			m_eBurnerState = BURNER::IDLE;
			m_bRender = false;
			break;
		case BURNER::START:
			m_tFrame.wstrStateKey = L"brn_start";
			m_bLoop = true;
			break;
		case BURNER::START_OFF:
			m_tFrame.wstrStateKey= L"brn_off";
			m_bLoop = true;
			break;
		case BURNER::ACCEL:
			m_tFrame.wstrStateKey = L"brn";
			m_bLoop = true;
			break;
		case BURNER::AFTER:
			m_tFrame.wstrStateKey = L"Aftbrn";
			m_bLoop = true;
			break;
		case BURNER::MEGA:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.wstrStateKey = L"Aftbrn_mega";
			m_bLoop = false;
			break;
		case BURNER::BURST:
			if (m_ePreBurnerState == BURNER::AFTER)
				return;
			m_tFrame.wstrStateKey= L"Aftbrn_burst";
			m_bLoop = false;
			break;
		}
		m_ePreBurnerState = m_eBurnerState;
		m_tFrame.fMaxFrame = (float)CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Player" + m_tFrame.wstrStateKey)->iMax_Index;
		m_tFrame.fStartFrame = 0;
		m_tFrame.fFrameSpeed = 10.f;
		if (m_eBurnerState != BURNER::IDLE)
			m_bRender = true;
	}
}
