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

void CCollider::Set_ColRect(_vec3 LeftTop, _vec3 RightBottom)
{
	m_ColRect.left = (LONG)LeftTop.x;
	m_ColRect.top= (LONG)LeftTop.y;
	m_ColRect.right = (LONG)RightBottom.x;
	m_ColRect.bottom = (LONG)RightBottom.y;
}

int CCollider::Update_GameObject()
{
	return 0;
}
