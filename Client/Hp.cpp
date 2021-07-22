#include "stdafx.h"
#include "Hp.h"
#include "Gui.h"
#include "Prefab_Manager.h"


CHp::CHp()
	:m_bSuper(false)
	,m_bUiRed(false)
	,m_bRed(false)
	,m_fRedTime(0.f)
	,m_iHp(0)
	,m_iPreHp(0)
{
}


CHp::~CHp()
{
}

CGameObject * CHp::Create(_vec3 _vPos)
{
	CGameObject* pInstance = new CHp;
	const ANIMATION* pAniInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_plate");
	pInstance->Set_Animation(pAniInfo);
	pInstance->Set_Pos(_vPos);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CHp::Ready_GameObject()
{

	Set_Frame(m_pAnimation);
	Set_Texture();
	m_eRenderId = RENDERID::UI;

	m_pHp_Plate_Red = CGui::Create(UI::HP_PLATE_RED);
	m_pHp_Glitch	= CGui::Create(UI::HP_GLITCH);
	m_pHp			= CGui::Create(UI::HP);
	m_pHp_Red		= CGui::Create(UI::HP_RED);

	m_tInfo.vSize = { 1.1f,1.1f,0.f };
	m_tColor = { 150,255,255,255 };

	Update_Pos();
	Update_Size();


	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pHp_Plate_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pHp);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pHp_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pHp_Glitch);

		
	return S_OK;
}

int CHp::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_iHp <= 0)
		m_iHp = 0;
	static_cast<CGui*>(m_pHp)->Select_Frame((float)m_iHp);
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bUiRed)
	{
		m_fRedTime += fTime;
		if (m_fRedTime > 0.1f)
		{
			static_cast<CUi*>(m_pHp_Red)->Select_Frame(m_pHp->Get_Frame().fStartFrame);
			static_cast<CUi*>(m_pHp_Red)->Set_Render(m_bRed);
			static_cast<CUi*>(m_pHp_Plate_Red)->Set_Render(m_bRed);
			m_bRender = true;
			m_bRed = !m_bRed;
			m_fRedTime = 0.f;
		}
	}
	else
	{
 		static_cast<CUi*>(m_pHp_Red)->Set_Render(false);
		static_cast<CUi*>(m_pHp_Plate_Red)->Set_Render(false);
	}
	return OBJ_NOEVENT;
}

void CHp::State_Change()
{
	switch (m_eState)
	{
	case PLAYER::IDLE:
		break;
	case PLAYER::HIT:
		static_cast<CUi*>(m_pHp_Glitch)->Set_Render(true);
		static_cast<CGui*>(m_pHp_Glitch)->Set_Action(true);
		break;
	}
}

void CHp::Update_Pos()
{
	m_pHp_Plate_Red->Set_Pos(m_tInfo.vPos);
	m_pHp_Glitch->Set_Pos(m_tInfo.vPos);
	m_pHp->Set_Pos(m_tInfo.vPos);
	m_pHp_Red->Set_Pos(m_tInfo.vPos);
}

void CHp::Update_Size()
{
	m_pHp_Plate_Red->Set_Size(m_tInfo.vSize);
	m_pHp_Glitch->Set_Size(m_tInfo.vSize);
	m_pHp->Set_Size(m_tInfo.vSize);
	m_pHp_Red->Set_Size(m_tInfo.vSize);
}
