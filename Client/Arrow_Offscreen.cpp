#include "stdafx.h"
#include "Arrow_Offscreen.h"
#include "Prefab_Manager.h"
#include "Gui.h"
CArrow_Offscreen::CArrow_Offscreen()
	:m_ePreArrowState(ARROW::END)
	, m_eArrowState(ARROW::END)
	, m_fPreFrame(0.f)
	, m_vecArrow(0.f, 0.f, 0.f)
	, m_vecRect(0.f, 0.f, 0.f)
	, m_vecArrowResult(0.f, 0.f, 0.f)
	, m_vecRectResult(0.f, 0.f, 0.f)
	,m_pArrow_Offscreen1(nullptr)
	,m_pArrow_Offscreen2(nullptr)
	,m_pArrow_Offscreen_Rect(nullptr)
	,m_pArrow_Icon(nullptr)
{
}


CArrow_Offscreen::~CArrow_Offscreen()
{
}

CGameObject * CArrow_Offscreen::Create(UI::ICON _eIcon)
{
	CGameObject* pInstance = new CArrow_Offscreen;
	const ANIMATION* pAniInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_offscreen");
	pInstance->Set_Animation(pAniInfo);
	static_cast<CArrow_Offscreen*>(pInstance)->Set_Icon(_eIcon);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CArrow_Offscreen::Ready_GameObject()
{
	m_eRenderId = RENDERID::UI;
	m_tInfo.vSize = { 1.f,1.f,0.f };
	m_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	m_tFrame.fMaxFrame = 4;
	m_tFrame.fStartFrame = 0;
	m_tColor = { 100,0,0,0 };

	m_vecArrow = { 0.35f,0.f,0.f };
	m_vecRect = { 0.40f,0.f,0.f };


	m_pArrow_Offscreen1		= CGui::Create(UI::ARROW_OFFSCREEN_DISTANCE1);
	m_pArrow_Offscreen2		= CGui::Create(UI::ARROW_OFFSCREEN_DISTANCE2);
	m_pArrow_Offscreen_Rect = CGui::Create(UI::ARROW_OFFSCREEN_RECT);
	m_pArrow_Icon			= CGui::Create(UI::ARROW_OFFSCREEN_ICON);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Offscreen1);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Offscreen2);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Offscreen_Rect);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Icon);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Marker_Rocket);

	m_bRender = false;
	static_cast<CGui*>(m_pArrow_Icon)->Select_Frame(float((int)m_eIcon));

	return S_OK;
}

void CArrow_Offscreen::State_Change()
{
	ArrowDistance_Change();
	ArrowDir_Change();
	ArrowPosition_Change();
	ArrowAlpha_Change();
}

void CArrow_Offscreen::ArrowDir_Change()
{
	if (m_fPreFrame != m_tFrame.fStartFrame)
	{
		static_cast<CUi*>(m_pArrow_Offscreen1)->Select_Frame(m_tFrame.fStartFrame);
		static_cast<CUi*>(m_pArrow_Offscreen2)->Select_Frame(m_tFrame.fStartFrame);
		static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Select_Frame(m_tFrame.fStartFrame);

		m_fPreFrame = m_tFrame.fStartFrame;
	}
}

void CArrow_Offscreen::ArrowDistance_Change()
{
	if (m_ePreArrowState != m_eArrowState)
	{
		switch (m_eArrowState)
		{
		case ARROW::DISTANS_FAR:
			static_cast<CUi*>(m_pArrow_Offscreen1)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Offscreen2)->Set_Render(false);

			static_cast<CUi*>(m_pArrow_Icon)->Set_Render(true);
			static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Set_Render(true);
			m_bRender = true;
			break;
		case ARROW::DISTANS_LEVEL0:
			static_cast<CUi*>(m_pArrow_Offscreen1)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Offscreen2)->Set_Render(false);

			static_cast<CUi*>(m_pArrow_Icon)->Set_Render(true);
			static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Set_Render(true);
			m_bRender = true;
			break;
		case ARROW::DISTANS_LEVEL1:
			static_cast<CUi*>(m_pArrow_Offscreen1)->Set_Render(true);
			static_cast<CUi*>(m_pArrow_Offscreen2)->Set_Render(false);

			static_cast<CUi*>(m_pArrow_Icon)->Set_Render(true);
			static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Set_Render(true);
			m_bRender = true;
			break;
		case ARROW::DISTANS_LEVEL2:
			static_cast<CUi*>(m_pArrow_Offscreen1)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Offscreen2)->Set_Render(true);

			static_cast<CUi*>(m_pArrow_Icon)->Set_Render(true);
			static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Set_Render(true);
			m_bRender = true;
			break;
		case ARROW::END:
			static_cast<CUi*>(m_pArrow_Offscreen1)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Offscreen2)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Icon)->Set_Render(false);
			static_cast<CUi*>(m_pArrow_Offscreen_Rect)->Set_Render(false);
			m_bRender = false;
		}

		m_ePreArrowState = m_eArrowState;
	}
}

void CArrow_Offscreen::ArrowPosition_Change()
{
	if (m_fPreFrame == 0.f)
	{
		m_vecArrowResult = m_vecArrow;
		m_vecRectResult = m_vecRect;
	}
	if (m_fPreFrame == 1.f)
	{
		m_vecArrowResult = -m_vecArrow;
		m_vecRectResult = -m_vecRect;
	}

	if (m_fPreFrame == 2.f)
	{
		m_vecArrowResult = { 0.f,m_vecArrow.x+ 0.3f,0.f };
		m_vecRectResult = { 0.f,m_vecRect.x -0.18f ,0.f };
	}

	if (m_fPreFrame == 3.f)
	{
		m_vecArrowResult = { 0.f,-m_vecArrow.x- 0.3f,0.f };
		m_vecRectResult = { 0.f,-m_vecRect.x+0.18f,0.f };
	}

	if (m_tInfo.vPos.x > WINCX)
		m_tInfo.vPos.x = WINCX - 15;
	if (m_tInfo.vPos.y > WINCY)
		m_tInfo.vPos.y = WINCY -15;

	if (m_tInfo.vPos.x < 0)
		m_tInfo.vPos.x = 15.f;
	if (m_tInfo.vPos.y < 0)
		m_tInfo.vPos.y = 15.f;


	m_pArrow_Offscreen1->Set_Pos(m_tInfo.vPos - (float)m_pTexInfo->tImageInfo.Width *m_vecArrowResult);
	m_pArrow_Offscreen2->Set_Pos(m_tInfo.vPos - (float)m_pTexInfo->tImageInfo.Width *m_vecArrowResult);
	m_pArrow_Offscreen_Rect->Set_Pos(m_tInfo.vPos + (float)m_pTexInfo->tImageInfo.Height *m_vecRectResult);
	m_pArrow_Icon->Set_Pos(m_tInfo.vPos + (float)m_pTexInfo->tImageInfo.Height *m_vecRectResult);

}

void CArrow_Offscreen::ArrowAlpha_Change()
{
	switch (m_eArrowState)
	{
	case ARROW::DISTANS_FAR:
	case ARROW::DISTANS_LEVEL0:
	case ARROW::DISTANS_LEVEL1:
		if (m_tColor.iAlpha > 40)
		{
			m_tColor.iAlpha = 100 - (int)(m_fDis*0.01);
			m_pArrow_Offscreen_Rect->Set_Color(MATCOLOR{ 255 - (int)(m_fDis*0.02),255,255,255 });
			m_pArrow_Icon->Set_Color(MATCOLOR{ 255 - (int)(m_fDis*0.02), 255, 255,255 });
		}
		break;
	case ARROW::DISTANS_LEVEL2:
		m_tColor.iAlpha = 100;
		m_pArrow_Offscreen_Rect->Set_Color(MATCOLOR{ 255,255,255,255 });
		m_pArrow_Icon->Set_Color(MATCOLOR{ 255,255,255,255 });
		break;
	}


}

int CArrow_Offscreen::Update_GameObject()
{
	if (m_bDead)
	{
		m_pArrow_Offscreen1->Set_Dead(true);
		m_pArrow_Offscreen2->Set_Dead(true);
		m_pArrow_Offscreen_Rect->Set_Dead(true);
		m_pArrow_Icon->Set_Dead(true);
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}
