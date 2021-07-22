#include "stdafx.h"
#include "WorngWay.h"
#include "Gui.h"
#include "Prefab_Manager.h"

CWorngWay::CWorngWay() :m_bEnd(false),m_bFlip(false), m_eWornWayState(WORNWAY::END)
{
}


CWorngWay::~CWorngWay()
{
}

CGameObject * CWorngWay::Create(_vec3 _vPos,bool _bFlip)
{
	CGameObject* pInstance = new CWorngWay;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	pInstance->Set_Pos(_vPos);
	static_cast<CWorngWay*>(pInstance)->Set_Flip(_bFlip);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CWorngWay::Ready_GameObject()
{

	m_eRenderId = RENDERID::UI;
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;

	m_pWrongWay_Start = CGui::Create(UI::WORNWAY_START);
	m_pWrongWay_Idle = CGui::Create(UI::WORNWAY_IDLE);
	m_pWrongWay_End = CGui::Create(UI::WORNWAY_END);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pWrongWay_Start);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pWrongWay_Idle);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pWrongWay_End);
	m_tInfo.vSize = { 1.1f,1.1f,0.f };

	m_pWrongWay_Start	->Set_Pos(m_tInfo.vPos);
	m_pWrongWay_Idle	->Set_Pos(m_tInfo.vPos);
	m_pWrongWay_End		->Set_Pos(m_tInfo.vPos);



	//¿ÞÂÊ
	if (!m_bFlip)
	{
		m_pWrongWay_Start->Set_Size(m_tInfo.vSize);
		m_pWrongWay_Idle->Set_Size(m_tInfo.vSize);
		m_pWrongWay_End->Set_Size(m_tInfo.vSize);
	}
	else
	{
		_vec3 vFlipSize = _vec3{ -m_tInfo.vSize.x,m_tInfo.vSize.y,0.f };
		m_pWrongWay_Start->Set_Size(vFlipSize);
		m_pWrongWay_Idle->Set_Size(vFlipSize);
		m_pWrongWay_End->Set_Size(vFlipSize);
	}

	return S_OK;
}

int CWorngWay::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eWornWayState == WORNWAY::WORNWAY_START)
	{
		if (static_cast<CGui*>(m_pWrongWay_Start)->Get_Start())
		{
			m_eWornWayState = WORNWAY::WORNWAY_IDLE;
		}
	}

	if (m_eWornWayState == WORNWAY::WORNWAY_END)
	{
		if (static_cast<CGui*>(m_pWrongWay_End)->Get_Start())
		{
			m_eWornWayState = WORNWAY::END;
			m_bEnd = false;
		}
	}

	State_Change();

	return OBJ_NOEVENT;
}

void CWorngWay::State_Change()
{
	
	switch (m_eWornWayState)
	{
	case WORNWAY::WORNWAY_START:
		static_cast<CGui*>(m_pWrongWay_Start)->Set_Render(true);
		static_cast<CGui*>(m_pWrongWay_Start)->Set_Action(true);
		m_bEnd = true;
		break;
	case WORNWAY::WORNWAY_IDLE:
		static_cast<CGui*>(m_pWrongWay_Start)->Set_Render(false);
		static_cast<CGui*>(m_pWrongWay_Start)->Set_Action(false);

		static_cast<CGui*>(m_pWrongWay_Idle)->Set_Render(true);
		static_cast<CGui*>(m_pWrongWay_Idle)->Set_Action(true);
		break;
	case WORNWAY::WORNWAY_END:
		if (m_bEnd)
		{
			static_cast<CGui*>(m_pWrongWay_Idle)->Set_Render(false);
			static_cast<CGui*>(m_pWrongWay_Idle)->Set_Action(false);

			static_cast<CGui*>(m_pWrongWay_End)->Set_Render(true);
			static_cast<CGui*>(m_pWrongWay_End)->Set_Action(true);
			break;
		}
	case WORNWAY::END:
		static_cast<CGui*>(m_pWrongWay_Start)->Set_Start(false);
		static_cast<CGui*>(m_pWrongWay_End)->Set_Render(false);
		static_cast<CGui*>(m_pWrongWay_End)->Set_Action(false);
		break;
	}

}
