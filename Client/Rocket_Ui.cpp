#include "stdafx.h"
#include "Rocket_Ui.h"
#include "Gui.h"
#include "Prefab_Manager.h"

Rocket_Ui::Rocket_Ui()
	: m_iIdx(0)
	, m_bPlate(false)
	, m_fRedTime(0.f)
	, m_bUiRed(false)
	
{
	for (int i = 0; i < 4; i++)
		m_bRocket[i] = false;
}


Rocket_Ui::~Rocket_Ui()
{
}

CGameObject * Rocket_Ui::Create(_vec3 _vPos)
{
	CGameObject* pInstance = new Rocket_Ui;
	const ANIMATION* pAniInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiRocket_plate");
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

HRESULT Rocket_Ui::Ready_GameObject()
{

	Set_Frame(m_pAnimation);
	Set_Texture();
	m_eRenderId = RENDERID::UI;
	m_pRocket_Plate_Red = CGui::Create(UI::ROCKET_PLATE_RED);

	for (int i = 0; i < 4; i++)
	{
		m_pRocket[i]		= CGui::Create(UI::ROCKET);
		m_pRocket_Red[i]	= CGui::Create(UI::ROCKET_RED);
	}
	m_tInfo.vSize = { 1.5f,1.5f,0.f };
	m_tColor = { 150,255,255,255 };


	Update_Pos();
	Update_Size();

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pRocket_Plate_Red);
	for (int i = 0; i < 4; i++)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pRocket[i]);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pRocket_Red[i]);
	}

	return S_OK;
}

int Rocket_Ui::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bUiRed)
	{
		m_fRedTime += fTime;
		if (m_fRedTime > 0.1f)
		{
			for (int i = 0; i < 4; i++)
			{
				static_cast<CUi*>(m_pRocket_Red[i])->Select_Frame(m_pRocket_Red[i]->Get_Frame().fStartFrame);
				static_cast<CUi*>(m_pRocket_Red[i])->Set_Render(m_bRed);
				static_cast<CUi*>(m_pRocket[i])->Set_Render(m_bRed);
			}
			static_cast<CUi*>(m_pRocket_Plate_Red)->Set_Render(m_bRed);
			m_bRender = m_bRed;
			m_bRed = !m_bRed;
			m_fRedTime = 0.f;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
			static_cast<CUi*>(m_pRocket_Red[i])->Set_Render(false);
		static_cast<CUi*>(m_pRocket_Plate_Red)->Set_Render(false);
	}
	return OBJ_NOEVENT;
}

void Rocket_Ui::State_Change()
{
	m_bPlate = false;
	for (int i = 0; i < 4; i++)
	{
		if (m_bRocket[i])
		{
			static_cast<CGui*>(m_pRocket[i])->Set_Action(true);
			m_bPlate = true;
		}
		else
		{
			static_cast<CGui*>(m_pRocket[i])->Set_Action(false);
			static_cast<CGui*>(m_pRocket[i])->Select_Frame(0.f);
		}
	}
	if (!m_bPlate) 
	{
		for (int i = 0; i < 4; i++)
			static_cast<CUi*>(m_pRocket[i])->Set_Render(false);
		m_bRender = false;
	}
	else
	{
		for (int i = 0; i < 4; i++)
			static_cast<CUi*>(m_pRocket[i])->Set_Render(true);
		m_bRender = true;
	}

}

void Rocket_Ui::Update_Pos()
{
	m_pRocket_Plate_Red->Set_Pos(m_tInfo.vPos);
	m_pRocket[0]->Set_Pos(_vec3{m_tInfo.vPos.x-m_pTexInfo->tImageInfo.Width*0.45f,m_tInfo.vPos.y,0.f });
	m_pRocket[1]->Set_Pos(_vec3{m_tInfo.vPos.x-m_pTexInfo->tImageInfo.Width*0.15f,m_tInfo.vPos.y,0.f });
	m_pRocket[2]->Set_Pos(_vec3{m_tInfo.vPos.x+m_pTexInfo->tImageInfo.Width*0.15f,m_tInfo.vPos.y,0.f });
	m_pRocket[3]->Set_Pos(_vec3{m_tInfo.vPos.x+m_pTexInfo->tImageInfo.Width*0.45f,m_tInfo.vPos.y,0.f });
	for (int i = 0; i < 4; i++)
	{
		m_pRocket_Red[i]->Set_Pos(m_pRocket[i]->Get_ObjInfo().vPos);
	}

}

void Rocket_Ui::Update_Size()
{
	m_pRocket_Plate_Red->Set_Size(m_tInfo.vSize);
	for (int i = 0; i < 4; i++)
	{
		m_pRocket[i]->Set_Size(m_tInfo.vSize);
		m_pRocket_Red[i]->Set_Size(m_tInfo.vSize);
	}
}
