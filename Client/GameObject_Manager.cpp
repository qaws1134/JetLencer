#include "stdafx.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
#include "Collider.h"
#include "Collision_Manager.h"
#include "Scroll_Manager.h"
#include "Jet_Enemy.h"
#include "BackGround.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager():m_bColRender(false), m_bTrueMod(false)
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager(); 
}

void CGameObject_Manager::Add_GameObject_Manager(OBJID::ID eID, CGameObject * pObject)
{
	if (OBJID::END <= eID || nullptr == pObject || 0 > eID)
		return; 

	m_listGameObject[eID].emplace_back(pObject); 
	for (int i = 0; i < pObject->GetColSize(); i++)
	{
		m_listGameObjectCollider[pObject->Get_ColVec(i)->Get_ColId()].emplace_back(pObject->Get_ColVec(i));
	}
}

void CGameObject_Manager::Update_GameObject_Manager()
{
	for (int i = 0 ; i < OBJID::END; ++i)
	{
	
		for (auto& iter = m_listGameObject[i].begin() ; iter != m_listGameObject[i].end() ; )
		{
			int iEvent = (*iter)->Update_GameObject(); 
				
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter); 
				iter = m_listGameObject[i].erase(iter); 
			}
			else 
				++iter; 
		}
	}
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pGameObject : m_listGameObject[i])
		{
			pGameObject->Late_Update_GameObject();

			m_listGameObjectRender[pGameObject->Get_RenderId()].emplace_back(pGameObject);
		}
	}
	for (int i = 0; i < COLLIDER::END; ++i)
	{
		for (auto& iter = m_listGameObjectCollider[i].begin(); iter != m_listGameObjectCollider[i].end(); )
		{
			int iEvent = (*iter)->Update_ColObject();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listGameObjectCollider[i].erase(iter);
			}
			else
				++iter;
		}
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_O))
	{
		m_bColRender = !m_bColRender;
	}

	CCollision_Manager::Collision_Player_Enemy_Bullet(m_listGameObjectCollider[COLLIDER::PLAYER], m_listGameObjectCollider[COLLIDER::ENEMY_BULLET]);

	CCollision_Manager::Collision_Enemy_Player_Bullet(m_listGameObjectCollider[COLLIDER::ENEMY], m_listGameObjectCollider[COLLIDER::PLAYER_BULLET]);

	CCollision_Manager::Collision_EnemyBeam(m_listGameObjectCollider[COLLIDER::PLAYER], m_listGameObjectCollider[COLLIDER::ENEMY_BULLET_BEAM]);

	CCollision_Manager::Collision_PlayerBeam(m_listGameObjectCollider[COLLIDER::ENEMY], m_listGameObjectCollider[COLLIDER::PLAYER_BULLET_BEAM]);
	CCollision_Manager::Collision_Enemy_Search(m_listGameObjectCollider[COLLIDER::ENEMY], m_listGameObjectCollider[COLLIDER::PLAYER_SEARCH]);
	CCollision_Manager::Collision_Rocket_Search(m_listGameObjectCollider[COLLIDER::ENEMY_BULLET], m_listGameObjectCollider[COLLIDER::PLAYER_SEARCH]);
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	_vec3 vScroll = CScroll_Manager::Get_Scroll();

	for (int i = 0; i < RENDERID::END; ++i)
	{

		for (auto& pGameObject : m_listGameObjectRender[i])
		{
			if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM4))
			{
				m_bTrueMod = !m_bTrueMod;
			}
			pGameObject->Set_AllTrueMod(m_bTrueMod);
			//if (pGameObject->Get_ObjInfo().vPos.x< -500 
			//	|| pGameObject->Get_ObjInfo().vPos.x>(WINCX + 500)
			//	|| pGameObject->Get_ObjInfo().vPos.y < -500
			//	|| pGameObject->Get_ObjInfo().vPos.y >(WINCY + 500))
			//	continue;
			pGameObject->Render_GameObject();
		}
		m_listGameObjectRender[i].clear();
	}
	if (m_bColRender)
	{
		for (int i = 0; i < COLLIDER::END; i++)
		{
			for (auto& pGameObject : m_listGameObjectCollider[i])
			{

				pGameObject->Render_GameObject();
			}
		}
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObject : m_listGameObject[i])
		{
			Safe_Delete(pObject);
		}
		m_listGameObject[i].clear();
	}
	for (int i = 0; i < COLLIDER::END; ++i)
	{
		for (auto& pObject : m_listGameObjectCollider[i])
		{
			Safe_Delete(pObject);
		}
		m_listGameObjectCollider[i].clear();
	}
}

CGameObject* CGameObject_Manager::Get_Target(CGameObject* _pObj, OBJID::ID _eID) const
{
	CGameObject*pTarget = nullptr;

	float fDis = 0.f;
	_vec3 vDis = {};
	float	fY = 0.f;
	float	fDia = 0.f;

	for (auto& pDst : m_listGameObject[_eID])
	{
		vDis = _pObj->Get_ObjInfo().vPos - pDst->Get_ObjInfo().vPos;

		fDia = D3DXVec3Length(&vDis);

		if (fDis > fDia || !pTarget)
		{
			pTarget = pDst;
			fDis = fDia;
		}
	}

	return pTarget;
}