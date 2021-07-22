#include "stdafx.h"
#include "Danger.h"
#include "Gui.h"
#include "Prefab_Manager.h"


CDanger::CDanger() : m_bStart(false),m_eDangerState(DANGER::END)
{
}


CDanger::~CDanger()
{
}

CGameObject * CDanger::Create(_vec3 _vPos)
{
	CGameObject* pInstance = new CDanger;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	pInstance->Set_Pos(_vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CDanger::Ready_GameObject()
{
	m_eRenderId = RENDERID::UI;
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;

	m_tInfo.vSize = { 1.1f,1.1f,0.f };
	m_tColor = { 255,255,255,255 };

	m_pDanger_Idle	= CGui::Create(UI::DANGER_IDLE);
	m_pDanger_Crit	= CGui::Create(UI::DANGER_CRIT);
	m_pDanger_Start = CGui::Create(UI::DANGER_START);
	m_pDanger_End	= CGui::Create(UI::DANGER_END);


	m_pDanger_Idle	->Set_Pos(m_tInfo.vPos);
	m_pDanger_Crit	->Set_Pos(m_tInfo.vPos);
	m_pDanger_Start	->Set_Pos(m_tInfo.vPos);
	m_pDanger_End	->Set_Pos(m_tInfo.vPos);

	m_pDanger_Idle	->Set_Size(m_tInfo.vSize);
	m_pDanger_Crit	->Set_Size(m_tInfo.vSize);
	m_pDanger_Start	->Set_Size(m_tInfo.vSize);
	m_pDanger_End	->Set_Size(m_tInfo.vSize);


	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pDanger_Idle);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pDanger_Crit);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pDanger_Start);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pDanger_End);

	return S_OK;
}

int CDanger::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eDangerState != DANGER::END)
	{
		//데인져 스타트가 끝나면 m_b스타트를 true로 반환
		if (m_eDangerState != DANGER::DANGER_END)
		{
			if (static_cast<CGui*>(m_pDanger_Start)->Get_Start())
			{
				m_eDangerState = DANGER::DANGER_IDLE;

				if (m_tFrame.fFrameSpeed < 150.f)
				{
					m_eDangerState = DANGER::DANGER_CRIT;
				}
			}
		}

	}

	if (static_cast<CGui*>(m_pDanger_End)->Get_Start())
	{
		m_eDangerState = DANGER::END;
		m_bStart = true;
	}


	return OBJ_NOEVENT;
}

void CDanger::State_Change()
{
	switch (m_eDangerState)
	{
	case DANGER::DANGER_START:
		static_cast<CGui*>(m_pDanger_Start)->Set_Render(true);
		static_cast<CGui*>(m_pDanger_Start)->Set_Action(true);
		m_bStart = false;
		break;
	case DANGER::DANGER_IDLE:
		static_cast<CGui*>(m_pDanger_Start)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_Crit)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_Crit)->Set_Action(false);
		static_cast<CGui*>(m_pDanger_Idle)->Set_Render(true);
		static_cast<CGui*>(m_pDanger_Idle)->Set_Action(true);
		static_cast<CGui*>(m_pDanger_Idle)->Set_FrameSpeed(((m_tFrame.fFrameSpeed*0.01f) -12.f)*-1.f);
		

		break;
	case DANGER::DANGER_CRIT:
		static_cast<CGui*>(m_pDanger_Crit)->Set_Render(true);
		static_cast<CGui*>(m_pDanger_Crit)->Set_Action(true);

		//아이들상태 false
		static_cast<CGui*>(m_pDanger_Idle)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_Idle)->Set_Action(false);

		//스타트 랜더 false
		static_cast<CGui*>(m_pDanger_Start)->Set_Render(false);
		break;
	case DANGER::DANGER_END:
		if (!m_bStart)
		{
			static_cast<CGui*>(m_pDanger_Idle)->Set_Render(false);
			static_cast<CGui*>(m_pDanger_Idle)->Set_Action(false);
			//크릿상태 false
			static_cast<CGui*>(m_pDanger_Crit)->Set_Render(false);
			static_cast<CGui*>(m_pDanger_Crit)->Set_Action(false);

			static_cast<CGui*>(m_pDanger_End)->Set_Render(true);
			static_cast<CGui*>(m_pDanger_End)->Set_Action(true);
		}
		break;
	case DANGER::END:
		static_cast<CGui*>(m_pDanger_End)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_End)->Set_Action(false);

		static_cast<CGui*>(m_pDanger_Crit)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_Idle)->Set_Render(false);
		static_cast<CGui*>(m_pDanger_Start)->Set_Start(false);
		static_cast<CGui*>(m_pDanger_Start)->Set_Render(false);
		break;
	}
	m_ePreDangerState = m_eDangerState;
}
