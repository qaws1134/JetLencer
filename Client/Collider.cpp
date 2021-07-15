#include "stdafx.h"
#include "Collider.h"


CCollider::CCollider()
{
	m_vOffsetDir = { 0.f,0.f,0.f };
}


CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_GameObject()
{

	return S_OK;
}

int CCollider::Update_ColObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (!m_pTarget)
		return OBJ_NOEVENT;


	m_tInfo.vPos = m_pTarget->Get_ObjInfo().vPos + m_vOffsetDir;

	return OBJ_NOEVENT;
}

void CCollider::Late_Update_GameObject()
{
}

void CCollider::Render_GameObject()
{
	WriteMatrix();
}

void CCollider::Release_GameObject()
{
}

void CCollider::State_Change()
{
}

void CCollider::Set_ColRect(float _fWidth, float _fHeight)
{
	m_ColRect.left		= (LONG)m_tInfo.vPos.x/* - _fWidth*0.5f*/;
	m_ColRect.top		= (LONG)m_tInfo.vPos.y /*- _fHeight*0.5f*/;
	m_ColRect.right		= (LONG)m_tInfo.vPos.x + _fWidth/**0.5f*/;
	m_ColRect.bottom	= (LONG)m_tInfo.vPos.y + _fHeight/**0.5f*/;
}

int CCollider::Update_GameObject()
{
	return 0;
}
