#include "stdafx.h"
#include "Bullet.h"
#include "Effect.h"
#include "Prefab_Manager.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release_GameObject();
}


int CBullet::Update_GameObject()
{
	if (m_bDeadEffect)
	{
		DeadEffect();
	}
	if (m_bDead)
	{
		return OBJ_DEAD;
	}


	Move();
	return OBJ_NOEVENT;
}

void CBullet::Late_Update_GameObject()
{
	if (m_tInfo.vPos.x > Map_Width || m_tInfo.vPos.x <0
		|| m_tInfo.vPos.y >Map_Height+500 || m_tInfo.vPos.y <0)
		m_bDead = true;
	FAILED_CHECK_RETURN(Set_Texture(), );
	State_Change();
}


void CBullet::Render_GameObject()
{
	WriteMatrix();
}

void CBullet::Release_GameObject()
{
}

