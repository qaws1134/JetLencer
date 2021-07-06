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
	if (m_bDead)
	{
		DeadEffect();
		return OBJ_DEAD;
	}
	Move();
	return OBJ_NOEVENT;
}

void CBullet::Late_Update_GameObject()
{
	//if (m_tInfo.vPos.x > WINCX - 100 || m_tInfo.vPos.y <100
	//	|| m_tInfo.vPos.y >WINCY-100 || m_tInfo.vPos.y < 100)
	//	m_bDead = true;
	State_Change();
	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CBullet::Render_GameObject()
{
	WriteMatrix();
}

void CBullet::Release_GameObject()
{
}

