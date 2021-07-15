#include "stdafx.h"
#include "Gui.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "Player.h"
CGui::CGui():m_bLoop(false) , m_bAction(false), m_bStart(false)
{
}


CGui::~CGui()
{
}

CGameObject * CGui::Create(const PLACEMENT * _pPlacement)
{
	CGameObject* pInstance = new CGui;
	pInstance->Set_Placement(_pPlacement);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

CGameObject * CGui::Create(UI::TYPE _eUiType)
{
	CGameObject* pInstance = new CGui;
	static_cast<CGui*>(pInstance)->Set_UiType(_eUiType);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}


HRESULT CGui::Ready_GameObject()
{
	m_eRenderId = RENDERID::UI;
	m_tInfo.vSize = { 1.2f,1.2f,0.f };
	InitGui();
	Set_Frame(m_pAnimation);
	return S_OK;
}

void CGui::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_bLoop)
	{
		if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fStartFrame = 0;
		}
		else if (m_tFrame.fStartFrame <= 0)
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame-1;
		}
	}
	else
	{
		if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
		}
		else if (m_tFrame.fStartFrame <= 0)
		{
			m_tFrame.fStartFrame = 0;
		}
	}
}



void CGui::State_Change()
{
	switch (m_eUiType)
	{
	case UI::ARROW_OFFSCREEN_BASE:
		break;
	case UI::ARROW_OFFSCREEN_DISTANCE1:
		break;
	case UI::ARROW_OFFSCREEN_DISTANCE2:
		break;
	case UI::ARROW_OFFSCREEN_RECT:
		break;
	case UI::ARROW_OFFSCREEN_ICON:
		break;
	case UI::AFTERBURNER:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 3.5f;
			m_fCenterX = 0;
			m_fCenterY = m_pTexInfo->tImageInfo.Height*0.5f;
		}
		else
		{
			m_tFrame.fFrameSpeed = -5.f;
		}
		Frame_Change();
		
		break;
	case UI::AFTERBURNER_RED:
		break;
	case UI::FLIP_PLATE:
		break;
	case UI::FLIP_PLATE_RED:
		break;
	case UI::FLIP_GLITCH:
		break;
	case UI::FLIP_VERTSPEED:
		m_bRender = true;
		if ((m_tFrame.fMaxFrame/m_tFrame.fStartFrame) > m_fOffset)		//화면의 몇분의 몇지점 
		{
			m_tFrame.fFrameSpeed = 5.f;
		}
		else
		{
			m_tFrame.fFrameSpeed = -5.f;
		}
		Frame_Change();
		break;
	case UI::FLIP_VERTSPEED_RED:
		break;
	case UI::ARROW:
		break;
	case UI::ARROW_RED:
		break;
	case UI::OVERHEAT_PLATE:
		break;
	case UI::OVERGEAT_PLATE_RED:
		break;
	case UI::OVERHEAT_TEXT:
		break;
	case UI::OVERHEAT_TEXT_RED:
		break;
	case UI::ROCKET:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = 5.f*fTime*60.f;
			m_fCenterX = 0.f;
			m_fCenterY = (float)m_pTexInfo->tImageInfo.Height;
		}
		Frame_Change();
		break;
	case UI::ROCKET_RED:
		break;
	case UI::SPECIAL_CHARGE:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = 10.f*60.f*fTime;
			m_fCenterX = 0.f;
			m_fCenterY = (float)m_pTexInfo->tImageInfo.Height;
		}
		else
		{
			m_tFrame.fFrameSpeed = -5.f;
		}
		Frame_Change();
		break;
	case UI::SPECIAL_PLATE:
		break;
	case UI::SPECIAL_PLATE_RED:
		break;
	case UI::SPECIAL_RELOAD_GLITCH:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = 20.f*fTime*60.f;
			m_tInfo.vDir = m_vTargetPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			m_tInfo.vPos += m_tInfo.vDir*100.f*fTime;
			Frame_Change();
		}
		break;
	case UI::SPECIAL_RELOAD_BAR:
		if (m_bStart)
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
			m_bStart = false;
		}
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = -(m_tFrame.fMaxFrame /static_cast<CPlayer*>( CGameObject_Manager::Get_Instance()->Get_Player())->Get_Special_Reload())*fTime * 60.f;
			m_fCenterX = 0.f;
			m_fCenterY = (float)m_pTexInfo->tImageInfo.Height;
		}
		Frame_Change();
		break;
	case UI::SPECIAL_RELOAD_READY_BAR:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = 20.f*fTime*60.f;
			m_tInfo.vDir = m_vTargetPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			m_tInfo.vPos += m_tInfo.vDir*100.f*fTime;
			Frame_Change();
		}
		break;
	case UI::SPECIAL_RELOAD_READY_BACKPLATE:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = 20.f*fTime*60.f;
			m_tInfo.vDir = m_vTargetPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			m_tInfo.vPos += m_tInfo.vDir*100.f*fTime;
			Frame_Change();
		}
		break;
	case UI::SPECIAL_RELOAD_READY_BACKPLATE_RED:
		break;
	default:
		break;
	}
}

void CGui::InitGui()
{
	switch (m_eUiType)
	{
	case UI::ARROW_OFFSCREEN_DISTANCE1:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_offscreen2_distance");
		m_bRender = false;
		break;
	case UI::ARROW_OFFSCREEN_DISTANCE2:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_offscreen_distance");
		m_bRender = false;
		break;
	case UI::ARROW_OFFSCREEN_RECT:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_offscreen3_distance");
		m_bRender = false;
		break;
	case UI::ARROW_OFFSCREEN_ICON:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_icon");
		m_bRender = false;
		break;
	case UI::AFTERBURNER:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiAfterburner");
		m_bCenter = true;
		break;
	case UI::AFTERBURNER_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiAfterburner_red");
		m_bRender = false;
		m_bCenter = true;
		break;
	case UI::FLIP_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_plate");
		break;
	case UI::FLIP_PLATE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_plate_red");
		m_bRender = false;
		break;
	case UI::FLIP_GLITCH:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_glitch");
		m_bRender = false;
		break;
	case UI::FLIP_VERTSPEED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_vertspeed");
		m_bLoop = true;
		m_bRender = false;
		break;
	case UI::FLIP_VERTSPEED_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_vertspeed_red");
		m_bRender = false;
		break;
	case UI::ARROW:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow");
		m_tColor = { 200,255,255,255 };
		break;
	case UI::ARROW_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiArrow_red");
		m_bRender = false;
		break;
	case UI::OVERHEAT_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiOverheat_plate");
		m_bRender = false;
		break;
	case UI::OVERGEAT_PLATE_RED :
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiOverheat_plate_red");
		m_bRender = false;
		break;
	case UI::OVERHEAT_TEXT:		
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiOverheat_text");
		m_bRender = false;
		break;
	case UI::OVERHEAT_TEXT_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiOverheat_text_red");
		m_bRender = false;
		break;
	case UI::ROCKET:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiRocket");
		m_bRender = false;
		break;
	case UI::ROCKET_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiRocket_red");
		m_bRender = false;
		break;
	case UI::SPECIAL_CHARGE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_charge");
		m_bCenter = true;
		m_bRender = false;
		break;
	case UI::SPECIAL_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_plate");
		m_tColor = { 150,255,255,255 };
		m_bRender = false;
		break;
	case UI::SPECIAL_PLATE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_plate_red");
		m_bRender = false;
		break;
	case UI::SPECIAL_RELOAD_GLITCH:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_reload_glitch");
		m_bRender = false;
		break;
	case UI::SPECIAL_RELOAD_BAR:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_reload");
		m_bCenter = true;
		m_bRender = false;
		break;
	case UI::SPECIAL_RELOAD_READY_BAR: 
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_reload_ready");
		m_bRender = false;
		break;
	case UI::SPECIAL_RELOAD_READY_BACKPLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_reload_ready_backplate");
		m_tColor = { 150,255,255,255 };
		m_bRender = false;
		break;
	case UI::SPECIAL_RELOAD_READY_BACKPLATE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiSpecial_reload_ready_backplate_red");
		m_bRender = false;
		break;
	default:
		break;
	}

}

int CGui::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}
