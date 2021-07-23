#include "stdafx.h"
#include "Gui.h"
#include "Prefab_Manager.h"
#include "Time_Manager.h"
#include "Player.h"
#include "Scroll_Manager.h"

CGui::CGui()
	:m_bLoop(false) 
	, m_bAction(false)
	, m_bStart(false)
	, m_fTimer(0.f)
	, m_fTime(0.f)
	, m_bRed(false)
	, m_bGreen(false)
	, m_fMaxSize(0.f)
	, m_fSizeIncrease(0.f)
	,m_fIncrease(0.f)
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

		if ( m_tFrame.fStartFrame<m_fOffset)		//화면의 몇분의 몇지점 
		{
			m_tFrame.fFrameSpeed = 15.f;
		}
		else
		{
			m_tFrame.fFrameSpeed = -15.f;
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
			m_tFrame.fFrameSpeed = -2.7f*fTime*60.f;
			m_fCenterX = 0.f;
			m_fCenterY = (float)m_pTexInfo->tImageInfo.Height;
		}
		else
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
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
			m_fReloadTime = static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_Special_Reload();
			m_bStart = false;
		}
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_tFrame.fFrameSpeed = -(m_tFrame.fMaxFrame / m_fReloadTime)*fTime * 60.f;
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
	case UI::HP:
		break;
	case UI::HP_GLITCH:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			m_fTime += fTime;
			if (m_fTime > m_fTimer)
				m_bAction = false;
			m_tFrame.fFrameSpeed = 30.f;
		}
		else
		{
			m_fTime = 0.f;
			m_tFrame.fStartFrame = 0.f;
			m_bRender = false;
		}
		Frame_Change();
		break;
	case UI::HP_PLATE:
		break;
	case UI::HP_PLATE_RED:
		break;
	case UI::HP_RED:
		break;
	case UI::DMGGRID:
		if (m_bAction)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			if (m_bGreen)
			{
				if (m_tColor.iRed > 10)
				{
					m_tColor.iRed -= (int)(fTime*800.f);
					m_tColor.iBlue -= (int)(fTime*800.f);
				}
				else
				{
					m_tColor.iRed = 0;
					m_tColor.iBlue = 0;
				}
			}
			if (m_bRed)
			{
				if (m_tColor.iGreen > 10)
				{
					m_tColor.iGreen -= (int)(fTime*800.f);
					m_tColor.iBlue -= (int)(fTime*800.f);
				}
				else
				{
					m_tColor.iGreen = 0;
					m_tColor.iBlue = 0;
				}
			}
			m_fTime += fTime;
			if (m_fTime > m_fTimer)
			{
				m_bRender = false;
				m_bAction = false;
				m_bRed = false;
				m_bGreen = false;
				m_fTime = 0.f;
			}
		}
		break;
	case UI::BOSS_HP:
		m_tFrame.fStartFrame = 0.f;
		m_fCenterX = 0.f;
		m_fCenterY = (float)(m_pTexInfo->tImageInfo.Height>>1);
		m_tInfo.vSize.x = m_fTargetSize;
		break;
	case UI::BOSS_HP_RED:
		m_tFrame.fStartFrame = 1.f;
		m_fCenterX = 0.f;
		m_fCenterY = (float)(m_pTexInfo->tImageInfo.Height>>1);
		if (m_tInfo.vSize.x > m_fTargetSize)
		{
			m_tInfo.vSize.x -= 0.01f;
		}
	
		break;
	case UI::BOSS_HP_PLATE:
		m_tFrame.fStartFrame = 2.f;
		m_fCenterX = 0.f;
		m_fCenterY = (float)(m_pTexInfo->tImageInfo.Height>>1);
		break;

	case UI::WORNWAY_START:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 20.f;
			if (m_tFrame.fStartFrame == m_tFrame.fMaxFrame - 1)
			{
				m_bStart = true;
				m_bAction = false;
			}
		}
		else
		{
			m_tFrame.fStartFrame = 0.f;
		}
		Frame_Change();
		break;
	case UI::WORNWAY_IDLE:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 20.f;
			Frame_Change();
		}
		break;
	case UI::WORNWAY_END:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 20.f;
			if (m_tFrame.fStartFrame == m_tFrame.fMaxFrame - 1)
			{
				m_bStart = true;
				m_bAction = false;
			}
			Frame_Change();
		}
		else
		{
			m_tFrame.fStartFrame = 0.f;
			m_bStart = false;
		}
		break;
	case UI::DANGER_START:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 20.f;
			Frame_Change();
			if (m_tFrame.fStartFrame == m_tFrame.fMaxFrame - 1)
			{
				m_bStart = true;
				m_bAction = false;
			}
		}
		else
		{
			m_tFrame.fStartFrame = 0.f;
		}
		break;
	case UI::DANGER_IDLE:
		if (m_bAction)
		{
			Frame_Change();
		}
		break;
	case UI::DANGER_END:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 20.f;
			Frame_Change();
			if (m_tFrame.fStartFrame == m_tFrame.fMaxFrame - 1)
			{
				m_bStart = true;
			}
		}
		else
		{
			//m_tFrame.fStartFrame = 0.f;
			m_bStart = false;
		}
		break;
	case UI::DANGER_CRIT:
		if (m_bAction)
		{
			m_tFrame.fFrameSpeed = 10.f;
			Frame_Change();
		}
		break;
	case UI::CLOUDE:

		if (m_bAction)
		{
			m_fIncrease += 0.03f;
			m_fSizeIncrease += 0.003f;

			m_vVelocity += _vec3{ 0.f,m_fIncrease,0.f };
			m_tInfo.vPos += m_vVelocity;

			m_tInfo.vSize += _vec3{ m_fSizeIncrease,0.f,0.f };

			if (m_tInfo.vPos.y > WINCY + 100.f)
			{
				m_bDead = true;
			}
		}
		break;
	case UI::LOGO:
		break;
	case UI::SCREENBLACK:
		break;
	case UI::BUTTON:
		break;
	case UI::FRAMEBACK:
		break;
	case UI::MARKER_SHINE:
		break;
	case UI::STAGEGUIDE:
		break;
	case UI::STAGEICON:
		break;

	case UI::LO_BACKPLATE:
		break;
	case UI::LO_CHARGEWEAPONE:
		break;
	case UI::LO_FRAME:
		break;
	case UI::LO_POINTER:
		m_tFrame.fFrameSpeed = 10.f;
		Frame_Change();
		break;
	case UI::LO_SUBWEAPON:
		break;
	case UI::LO_TEXTLINERED:

		break;
	case UI::LO_TEXTLINEYELLO:
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
	case UI::ROCKET_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiRocket_plate");
		m_bRender = false;
		break;
	case UI::ROCKET_PLATE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiRocket_plate_red");
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
	case UI::HP:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_idle");
		m_bRender = true;
		break;
	case UI::HP_GLITCH:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_glitch");
		m_bRender = false;
		m_fTimer = 0.5f;
		break;
	case UI::HP_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_plate");
		m_bRender = true;
		break;
	case UI::HP_PLATE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_plate_red");
		m_bRender = false;
		break;
	case UI::HP_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiHp_red");
		m_bRender = false;
		break;
	case UI::DMGGRID:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiDamage_grid");
		m_bRender = false;
		break;
	case UI::BOSS_HP:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiBosshp_idle");
		m_bRender = true;
		m_bCenter = true;
		m_fMaxSize = 1.5f;
		m_fTargetSize = m_fMaxSize;
		m_tInfo.vSize = { m_fMaxSize,1.f,0.f };
		m_tInfo.vPos = {float(WINCX>>2)+10.f,float(WINCY>>3),0.f};
		break;
	case UI::BOSS_HP_PLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiBosshp_idle");
		m_bRender = true;
		m_bCenter = true;
		m_fMaxSize = 1.5f;
		m_tInfo.vSize = { m_fMaxSize,1.f,0.f };
		m_tInfo.vPos = { float(WINCX >> 2),float(WINCY >> 3),0.f };
		break;
	case UI::BOSS_HP_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiBosshp_idle");
		m_bRender = true;
		m_bCenter = true;
		m_fMaxSize = 1.5f;
		m_fTargetSize = m_fMaxSize;
		m_tInfo.vSize = { m_fMaxSize,1.f,0.f };
		m_tInfo.vPos = { float(WINCX >> 2)+9.f,float(WINCY >> 3),0.f };
		break;
	case UI::MOUSE_HIT:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Mouse_Hit");
		m_tInfo.vSize = { 1.5f,1.5f,0.f };
		m_bRender = false;
		break;
	case UI::WORNWAY_START:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiWornway_start");
		m_bRender = false;
		break;
	case UI::WORNWAY_IDLE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiWornway_idle");
		m_bLoop = true;
		m_bRender = false;
		break;
	case UI::WORNWAY_END:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiWornway_end");
		m_bRender = false;
		break;
	case UI::DANGER_START:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiDanger_start");
		m_bRender = false;
		break;
	case UI::DANGER_IDLE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiDanger_idle");
		m_bLoop = true;
		m_bRender = false;
		break;
	case UI::DANGER_END:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiDanger_end");
		m_bRender = false;
		break;
	case UI::DANGER_CRIT:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiDanger_crit");
		m_bRender = false;
		break;

	case UI::MENUBACKGROUND:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuBackground");
		m_tInfo.vSize = { 1.3f,1.f,0.f };
		m_tInfo.vPos = { float(WINCX >> 1),float(WINCY >> 1) - 150.f,0.f };
		m_eRenderId = RENDERID::BACKGROUND;
		break;
	case UI::CLOUDE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuCloude");
		m_tInfo.vSize = { 1.3f,0.7f,0.f };
		m_vVelocity = { 0.f,0.f,0.f };
		m_fIncrease = 0.f;

		m_eRenderId = RENDERID::MOVE_BACKGROUND1;
		break;
	case UI::LOGO:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuLogo");
		break;
	case UI::SCREENBLACK:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuScreenBlack");
		break;
	case UI::BUTTON:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuButton");
		m_tInfo.vSize = { 1.f,1.5f,0.f };
		break;
	case UI::FRAMEBACK:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuFrameBack");
		break;
	case UI::MARKER_SHINE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuMarker_Shine");
		break;
	case UI::STAGEGUIDE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuStageGuide");
		break;
	case UI::STAGEICON:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"MenuStageIcon");
		break;

	case UI::LO_BACKPLATE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutBackplate");
		break;
	case UI::LO_CHARGEWEAPONE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutChargeWeapon");
		break;
	case UI::LO_FRAME:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutFrame");
		break;
	case UI::LO_POINTER:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutPonter");
		m_bLoop = true;
		break;
	case UI::LO_SUBWEAPON:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutSubWeapon");

		break;
	case UI::LO_MODULE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutModule");
		break;
	case UI::LO_TEXTLINERED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutTextLineRed");
		break;
	case UI::LO_TEXTLINEYELLO:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutTextLineYellow");
		break;
	case UI::LO_BACKGROUND:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"LoadOutBackGround");
		break;

	}

}

int CGui::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}
