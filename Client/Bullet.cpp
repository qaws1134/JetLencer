#include "stdafx.h"
#include "Bullet.h"
#include "Effect.h"
#include "Prefab_Manager.h"
#include "Collider.h"
#include "Spawn_Manager.h"
CBullet::CBullet() 
	: m_fRemoveTIme(0.f)
	, m_fRemoveSpeed(0.f)
	, m_bRemove(false)
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
		return OBJ_DEAD;
	}
	if (m_bDeadEffect)
	{
		DeadEffect();
	}

	Set_Texture();
	Move();
	return OBJ_NOEVENT;
}


void CBullet::Render_GameObject()
{
	WriteMatrix();
}

void CBullet::Release_GameObject()
{

}
