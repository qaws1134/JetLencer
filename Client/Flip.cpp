#include "stdafx.h"
#include "Flip.h"
#include "Gui.h"
#include"Prefab_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"

CFlip::CFlip()
	: m_pAfterBurner(nullptr)
	, m_pAfterBurner_Red(nullptr)
	, m_pArrow(nullptr)
	, m_pArrow_Red(nullptr)
	, m_pFlip_Glitch(nullptr)
	, m_pVertSpeed(nullptr)
	, m_pVertSpeed_Red(nullptr)
	, m_pFlip_Red(nullptr)
	, m_ePlayerState(PLAYER::END)
	, m_bYflip(false)
	, m_bOverHeat(false)
	, m_bRed(false)	
	, m_fRedTime(0.f)
	,m_fAfterBurnerX(0.f)
	,m_fAfterBurnerY(0.f)
	,m_fAfterBurnerRedY(0.f)
	,m_fArrowX(0.f)
	,m_fVertX(0.f)
	,m_fVertY(0.f)
	,m_fSpecial_ChargeX(0.f)
	,m_fSpecial_ChargeY(0.f)
	,m_fSpecial_PlateX(0.f)
	,m_fSpecial_PlateY(0.f)
	,m_fSpecial_Reload_BarX(0.f)
	,m_fSpecial_Reload_BarY(0.f)
	, m_fSpecial_Reload_PlateX(0.f)
	,m_fSpecial_Reload_PlateY(0.f)
	, m_bReload(false)
	, m_bStart(false)
	, m_bGlitch(false)

{
}


CFlip::~CFlip()
{

}

CGameObject * CFlip::Create(_vec3 _vPos, bool _bYflip)
{
	CGameObject* pInstance = new CFlip;
	const ANIMATION* pAniInfo = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiFlip_plate");
	pInstance->Set_Animation(pAniInfo);
	pInstance->Set_Pos(_vPos);
	static_cast<CFlip*>(pInstance)->Set_YFlip(_bYflip);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CFlip::Ready_GameObject()
{
	Set_Frame(m_pAnimation);
	Set_Texture();
	m_eRenderId = RENDERID::UI;
	if(!m_bYflip)
		m_tInfo.vSize = { 1.5f,1.5f,0.f };
	else
		m_tInfo.vSize = { -1.5f,1.5f,0.f };
	m_tColor = { 150,255,255,255 };
	m_fRedTime = 0.f;


	Init_Create();
	Update_Pos();
	Update_Size();

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pFlip_Glitch);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pFlip_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pAfterBurner);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pAfterBurner_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pArrow_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pVertSpeed);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pVertSpeed_Red);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pOverHeat_Plate);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pOverHeat_Plate_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pOverHeat_Text);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pOverHeat_Text_Red);

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Plate);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Plate_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Charge);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Reload_Ready_Backplate);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Reload_Ready_Backplate_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Reload_Glitch);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Reload_Bar);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), m_pSpecial_Reload_Ready_Bar);


	return S_OK;
}

//위치 변경 -> 상시
//스테이트 체인지 -> 글릿치 -> 데드이펙트임 
//에프터버너인지
//오버히트인지


//에프터버너 시 플립 간격 줄여주기 - > 처음엔 확줄었다가 점점 조금줄음
//오버히트시 좀더 멀리 떨어졌다가 다시 원위치 
//각 위치는 Ui 생성하는 곳에서 관리 - > ui매니저  야발 -> 플레이어에서 관리 개꿀 


int CFlip::Update_GameObject()
{
	if(m_bDead)
	{
		return OBJ_DEAD;
	}

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (m_bOverHeat)
	{
		static_cast<CUi*>(m_pOverHeat_Plate)	->Set_Render(true);
		static_cast<CUi*>(m_pOverHeat_Text)		->Set_Render(true);
		m_fRedTime += fTime;
		if (m_fRedTime > 0.1f)
		{
			static_cast<CUi*>(m_pAfterBurner_Red)->
				Select_Frame(m_pAfterBurner->Get_Frame().fStartFrame);
			static_cast<CUi*>(m_pVertSpeed_Red)->
				Select_Frame(m_pVertSpeed->Get_Frame().fStartFrame);

			static_cast<CUi*>(m_pAfterBurner_Red)	->Set_Render(m_bRed);
			static_cast<CUi*>(m_pFlip_Red)			->Set_Render(m_bRed);
			static_cast<CUi*>(m_pArrow_Red)			->Set_Render(m_bRed);
			static_cast<CUi*>(m_pVertSpeed_Red)		->Set_Render(m_bRed);
			static_cast<CUi*>(m_pOverHeat_Plate_Red)->Set_Render(m_bRed);
			static_cast<CUi*>(m_pOverHeat_Text_Red)	->Set_Render(m_bRed);


			if (m_ePlayerWeaponState == PLAYER::SPECIAL_RELOAD_START)
			{
				static_cast<CUi*>(m_pSpecial_Reload_Ready_Backplate_Red)->Set_Render(m_bRed);
			}
			else if (m_ePlayerWeaponState == PLAYER::SPECIAL_CHARGE)
			{
				static_cast<CUi*>(m_pSpecial_Plate_Red)->Set_Render(m_bRed);
			}

			m_bRed = !m_bRed;
			m_fRedTime = 0.f;
		}
	}
	else
	{
		static_cast<CUi*>(m_pAfterBurner_Red)->Set_Render(false);
		static_cast<CUi*>(m_pFlip_Red)->Set_Render(false);
		static_cast<CUi*>(m_pArrow_Red)->Set_Render(false);
		static_cast<CUi*>(m_pVertSpeed_Red)->Set_Render(false);
		static_cast<CUi*>(m_pOverHeat_Plate)->Set_Render(false);
		static_cast<CUi*>(m_pOverHeat_Plate_Red)->Set_Render(false);
		static_cast<CUi*>(m_pOverHeat_Text)->Set_Render(false);
		static_cast<CUi*>(m_pOverHeat_Text_Red)->Set_Render(false);
	}

	Update_Pos();
	VertUpdate();
	HoriUpdate();

	return OBJ_NOEVENT;
}

void CFlip::State_Change()
{
	switch (m_ePlayerState)
	{
	case PLAYER::AFTER_BURNUR:
		static_cast<CGui*>(m_pAfterBurner)->Set_Action(true);
		break;
	case PLAYER::OVERHEAT:
		m_bOverHeat = true;
	default:
		static_cast<CGui*>(m_pAfterBurner)->Set_Action(false);
		break;
	}

	switch (m_ePlayerWeaponState)
	{
	case PLAYER::SPECIAL_CHARGE:
		m_bReload = false;
		m_bGlitch = false;
		static_cast<CUi*>(m_pSpecial_Charge)->Set_Render(true);
		static_cast<CUi*>(m_pSpecial_Plate)->Set_Render(true);
		static_cast<CGui*>(m_pSpecial_Charge)->Set_Action(true);

		static_cast<CGui*>(m_pSpecial_Reload_Ready_Bar)->Set_Action(false);
		static_cast<CUi*>(m_pSpecial_Reload_Ready_Bar)->Set_Render(false);
		static_cast<CGui*>(m_pSpecial_Reload_Ready_Bar)->Select_Frame(0.f);

		static_cast<CUi*>(m_pSpecial_Reload_Ready_Backplate)->Set_Render(false);
		static_cast<CGui*>(m_pSpecial_Reload_Ready_Backplate)->Set_Action(false);
		static_cast<CGui*>(m_pSpecial_Reload_Ready_Backplate)->Select_Frame(0.f);
		static_cast<CGui*>(m_pSpecial_Reload_Bar)->Set_Start(true);
		break;
	case PLAYER::SPECIAL_RELOAD_START:
		static_cast<CUi*>(m_pSpecial_Reload_Bar)->Set_Render(true);
		static_cast<CUi*>(m_pSpecial_Reload_Glitch)->Set_Render(true);
		static_cast<CUi*>(m_pSpecial_Reload_Ready_Backplate)->Set_Render(true);


		static_cast<CGui*>(m_pSpecial_Reload_Bar)->Set_Action(true);
		static_cast<CGui*>(m_pSpecial_Reload_Glitch)->Set_Action(true);

		static_cast<CGui*>(m_pSpecial_Charge)->Set_Action(false);
		static_cast<CUi*>(m_pSpecial_Charge)->Select_Frame(0.f);
		static_cast<CUi*>(m_pSpecial_Charge)->Set_Render(false);
		static_cast<CUi*>(m_pSpecial_Plate)->Set_Render(false);

		if (!m_bYflip)
		{
			static_cast<CGui*>(m_pSpecial_Reload_Glitch)->Set_TargetPos(_vec3{ m_tInfo.vPos.x + m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
		}
		else
		{
			static_cast<CGui*>(m_pSpecial_Reload_Glitch)->Set_TargetPos(_vec3{ m_tInfo.vPos.x - m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
		}
		m_bGlitch = true;
		break;
	case PLAYER::SPECIAL_RELOAD_END:
		static_cast<CUi*> (m_pSpecial_Reload_Ready_Bar)->Set_Render(true);

		static_cast<CGui*>(m_pSpecial_Reload_Ready_Bar)->Set_Action(true);
		static_cast<CGui*>(m_pSpecial_Reload_Ready_Backplate)->Set_Action(true);

		static_cast<CUi*>(m_pSpecial_Reload_Bar)->Set_Render(false);
		static_cast<CUi*>(m_pSpecial_Reload_Glitch)->Set_Render(false)
			;
		static_cast<CGui*>(m_pSpecial_Reload_Bar)->Set_Action(false);
		static_cast<CGui*>(m_pSpecial_Reload_Glitch)->Set_Action(false);

		static_cast<CGui*>(m_pSpecial_Reload_Bar)->Select_Frame(0.f);
		static_cast<CUi*>(m_pSpecial_Reload_Glitch)->Select_Frame(0.f);

		if (!m_bYflip)
		{
			static_cast<CGui*>(m_pSpecial_Reload_Ready_Bar)->Set_TargetPos(_vec3{ m_tInfo.vPos.x + m_pTexInfo->tImageInfo.Width*0.62f,m_tInfo.vPos.y + m_pTexInfo->tImageInfo.Height*0.49f ,0.f });
			static_cast<CGui*>(m_pSpecial_Reload_Ready_Backplate)->Set_TargetPos(_vec3{ m_tInfo.vPos.x + m_pTexInfo->tImageInfo.Width*0.62f,m_tInfo.vPos.y + m_pTexInfo->tImageInfo.Height*0.49f ,0.f });
		}
		else
		{
			static_cast<CGui*>(m_pSpecial_Reload_Ready_Bar)->Set_TargetPos(_vec3{ m_tInfo.vPos.x - m_pTexInfo->tImageInfo.Width*0.62f,m_tInfo.vPos.y + m_pTexInfo->tImageInfo.Height*0.49f ,0.f });
			static_cast<CGui*>(m_pSpecial_Reload_Ready_Backplate)->Set_TargetPos(_vec3{ m_tInfo.vPos.x - m_pTexInfo->tImageInfo.Width*0.62f,m_tInfo.vPos.y + m_pTexInfo->tImageInfo.Height*0.49f ,0.f });
		}
		m_bReload = true;
		break;
	}
}

void CFlip::VertUpdate()
{
	static_cast<CUi*>(m_pVertSpeed)->Set_Offset(m_pVertSpeed->Get_Frame().fMaxFrame*CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vPos.x/ Map_Width);
}

void CFlip::HoriUpdate()
{

	m_pArrow->Set_Pos(_vec3{ m_pArrow->Get_ObjInfo().vPos.x,
		(WINCY*0.63f)+m_pTexInfo->tImageInfo.Height *CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vPos.y / Map_Height,0.f });
	m_pArrow_Red->Set_Pos(_vec3{ m_pArrow->Get_ObjInfo().vPos.x,
		(WINCY*0.63f) + +m_pTexInfo->tImageInfo.Height *CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vPos.y / Map_Height,0.f });

}

void CFlip::Update_Size()
{
	if (!m_bYflip)
	{
		m_pVertSpeed->Set_Size(m_tInfo.vSize);
		m_pVertSpeed_Red->Set_Size(m_tInfo.vSize);
	}
	else
	{
		m_pVertSpeed->Set_Size(-m_tInfo.vSize);
		m_pVertSpeed_Red->Set_Size(-m_tInfo.vSize);
	}

	m_pAfterBurner->Set_Size(m_tInfo.vSize);
	m_pAfterBurner_Red->Set_Size(_vec3{ m_tInfo.vSize.x,m_tInfo.vSize.y + 0.5f,m_tInfo.vSize.z });

	m_pFlip_Glitch->Set_Size(m_tInfo.vSize);
	m_pFlip_Red->Set_Size(m_tInfo.vSize);

	m_pArrow->Set_Size(m_tInfo.vSize);
	m_pArrow_Red->Set_Size(m_tInfo.vSize);


	m_pOverHeat_Plate		->Set_Size(m_tInfo.vSize);
	m_pOverHeat_Plate_Red	->Set_Size(m_tInfo.vSize);
	m_pOverHeat_Text		->Set_Size(m_tInfo.vSize);
	m_pOverHeat_Text_Red	->Set_Size(m_tInfo.vSize);



	m_pSpecial_Charge						->Set_Size(m_tInfo.vSize);
	m_pSpecial_Plate						->Set_Size(m_tInfo.vSize);
	m_pSpecial_Plate_Red					->Set_Size(m_tInfo.vSize);
	m_pSpecial_Reload_Glitch				->Set_Size(m_tInfo.vSize);
	m_pSpecial_Reload_Bar					->Set_Size(m_tInfo.vSize);
	m_pSpecial_Reload_Ready_Bar				->Set_Size(m_tInfo.vSize);
	m_pSpecial_Reload_Ready_Backplate		->Set_Size(m_tInfo.vSize);
	m_pSpecial_Reload_Ready_Backplate_Red	->Set_Size(m_tInfo.vSize);



}

void CFlip::Init_Create()
{
	m_pAfterBurner		= CGui::Create(UI::AFTERBURNER);
	m_pAfterBurner_Red	= CGui::Create(UI::AFTERBURNER_RED);

	m_pFlip_Glitch		= CGui::Create(UI::FLIP_GLITCH);
	m_pFlip_Red			= CGui::Create(UI::FLIP_PLATE_RED);

	m_pArrow			= CGui::Create(UI::ARROW);
	m_pArrow_Red		= CGui::Create(UI::ARROW_RED);

	m_pVertSpeed		= CGui::Create(UI::FLIP_VERTSPEED);
	m_pVertSpeed_Red	= CGui::Create(UI::FLIP_VERTSPEED_RED);

	m_pOverHeat_Plate		= CGui::Create(UI::OVERHEAT_PLATE);
	m_pOverHeat_Plate_Red	= CGui::Create(UI::OVERGEAT_PLATE_RED);
	m_pOverHeat_Text_Red	= CGui::Create(UI::OVERHEAT_TEXT_RED);
	m_pOverHeat_Text		= CGui::Create(UI::OVERHEAT_TEXT);

	m_pSpecial_Charge						= CGui::Create(UI::SPECIAL_CHARGE);
	m_pSpecial_Plate						= CGui::Create(UI::SPECIAL_PLATE);
	m_pSpecial_Plate_Red					= CGui::Create(UI::SPECIAL_PLATE_RED);
	m_pSpecial_Reload_Glitch				= CGui::Create(UI::SPECIAL_RELOAD_GLITCH);
	m_pSpecial_Reload_Bar					= CGui::Create(UI::SPECIAL_RELOAD_BAR);
	m_pSpecial_Reload_Ready_Bar				= CGui::Create(UI::SPECIAL_RELOAD_READY_BAR);
	m_pSpecial_Reload_Ready_Backplate		= CGui::Create(UI::SPECIAL_RELOAD_READY_BACKPLATE);
	m_pSpecial_Reload_Ready_Backplate_Red	= CGui::Create(UI::SPECIAL_RELOAD_READY_BACKPLATE_RED);



	//위치 오프셋 초기화
	m_fAfterBurnerX = m_pTexInfo->tImageInfo.Width*0.75f;
	m_fAfterBurnerY = m_pTexInfo->tImageInfo.Height* 0.65f;
	m_fAfterBurnerRedY = m_pTexInfo->tImageInfo.Height*0.58f;

	m_fArrowX = m_pTexInfo->tImageInfo.Width*0.65f;

	m_fVertX = m_pTexInfo->tImageInfo.Width*0.1f;
	m_fVertY = m_pTexInfo->tImageInfo.Height*0.5f;

	m_fSpecial_ChargeX = m_pTexInfo->tImageInfo.Width* 0.54f;
	m_fSpecial_ChargeY = m_pTexInfo->tImageInfo.Height*1.2f;

	m_fSpecial_PlateX = m_pTexInfo->tImageInfo.Width* 0.85f;
	m_fSpecial_PlateY = m_pTexInfo->tImageInfo.Height*0.8f;

	m_fSpecial_Reload_PlateX = m_pTexInfo->tImageInfo.Width * 0.68f;
	m_fSpecial_Reload_PlateY = m_pTexInfo->tImageInfo.Height* 0.6f;

	m_fSpecial_Reload_BarX = m_pTexInfo->tImageInfo.Width * 0.53f;
	m_fSpecial_Reload_BarY = m_pTexInfo->tImageInfo.Height* 0.9f;

	m_fOverHeat_PlateX = m_pTexInfo->tImageInfo.Width*0.83f;
	m_fOverHeat_PlateY = m_pTexInfo->tImageInfo.Height*0.85f;


}


void CFlip::Update_Pos()
{
	//왼쪽 위치 셋팅
	if (!m_bYflip)
	{

		m_pAfterBurner			->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fAfterBurnerX ,m_tInfo.vPos.y + m_fAfterBurnerY,0.f });
		m_pAfterBurner_Red		->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fAfterBurnerX ,m_tInfo.vPos.y + m_fAfterBurnerRedY,0.f });
		m_pArrow				->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fArrowX ,m_tInfo.vPos.y,0.f });
		m_pArrow_Red			->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fArrowX ,m_tInfo.vPos.y,0.f });

		m_pVertSpeed			->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fVertX,(m_tInfo.vPos.y + m_fVertY),0.f });
		m_pVertSpeed_Red		->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fVertX,(m_tInfo.vPos.y + m_fVertY),0.f });
		

		m_pSpecial_Charge		->Set_Pos(_vec3{m_tInfo.vPos.x+ m_fSpecial_ChargeX	   ,m_tInfo.vPos.y  + m_fSpecial_ChargeY,0.f});
		m_pSpecial_Plate		->Set_Pos(_vec3{m_tInfo.vPos.x+ m_fSpecial_PlateX	   ,m_tInfo.vPos.y  + m_fSpecial_PlateY,0.f});
		m_pSpecial_Plate_Red	->Set_Pos(_vec3{m_tInfo.vPos.x+ m_fSpecial_PlateX	   ,m_tInfo.vPos.y  + m_fSpecial_PlateY,0.f});

		m_pOverHeat_Plate		->Set_Pos(_vec3{m_tInfo.vPos.x-m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Plate_Red	->Set_Pos(_vec3{m_tInfo.vPos.x-m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Text		->Set_Pos(_vec3{m_tInfo.vPos.x-m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Text_Red	->Set_Pos(_vec3{m_tInfo.vPos.x-m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});


		if (!m_bGlitch)
		{
			m_pSpecial_Reload_Glitch->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fSpecial_PlateX	   ,m_tInfo.vPos.y + m_fSpecial_PlateY,0.f });
		}

		if (!m_bReload)
		{
			m_pSpecial_Reload_Bar->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fSpecial_Reload_BarX, m_tInfo.vPos.y + m_fSpecial_Reload_BarY,0.f });
			m_pSpecial_Reload_Ready_Bar->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
			m_pSpecial_Reload_Ready_Backplate->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
			m_pSpecial_Reload_Ready_Backplate_Red->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
		}

	}
	else
	{
		//오른쪽 
		m_pAfterBurner			->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fAfterBurnerX ,m_tInfo.vPos.y + m_fAfterBurnerY,0.f });
		m_pAfterBurner_Red		->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fAfterBurnerX ,m_tInfo.vPos.y + m_fAfterBurnerRedY,0.f });
		m_pArrow				->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fArrowX ,m_tInfo.vPos.y,0.f });
		m_pArrow_Red			->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fArrowX ,m_tInfo.vPos.y,0.f });

		m_pVertSpeed			->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fVertX,(m_tInfo.vPos.y + m_fVertY),0.f });
		m_pVertSpeed_Red		->Set_Pos(_vec3{ m_tInfo.vPos.x + m_fVertX,(m_tInfo.vPos.y + m_fVertY),0.f });


		m_pSpecial_Charge		->Set_Pos(_vec3{m_tInfo.vPos.x- m_fSpecial_ChargeX     , m_tInfo.vPos.y   + m_fSpecial_ChargeY,0.f});
		m_pSpecial_Plate		->Set_Pos(_vec3{m_tInfo.vPos.x- m_fSpecial_PlateX      , m_tInfo.vPos.y   + m_fSpecial_PlateY,0.f});
		m_pSpecial_Plate_Red	->Set_Pos(_vec3{m_tInfo.vPos.x- m_fSpecial_PlateX      , m_tInfo.vPos.y   + m_fSpecial_PlateY,0.f});


		m_pOverHeat_Plate		->Set_Pos(_vec3{m_tInfo.vPos.x+m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Plate_Red	->Set_Pos(_vec3{m_tInfo.vPos.x+m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Text		->Set_Pos(_vec3{m_tInfo.vPos.x+m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});
		m_pOverHeat_Text_Red	->Set_Pos(_vec3{m_tInfo.vPos.x+m_fOverHeat_PlateX,m_tInfo.vPos.y+m_fOverHeat_PlateY,0.f});

		static_cast<CGui*>(m_pOverHeat_Text)->Select_Frame(1.f);
		static_cast<CGui*>(m_pOverHeat_Text_Red)->Select_Frame(1.f);
		if (!m_bGlitch)
		{
			m_pSpecial_Reload_Glitch->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fSpecial_PlateX , m_tInfo.vPos.y + m_fSpecial_PlateY,0.f });
		}
		if (!m_bReload)
		{
			m_pSpecial_Reload_Bar			->Set_Pos(_vec3{m_tInfo.vPos.x- m_fSpecial_Reload_BarX , m_tInfo.vPos.y  + m_fSpecial_Reload_BarY,0.f});
			m_pSpecial_Reload_Ready_Bar		->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fSpecial_Reload_PlateX , m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
			m_pSpecial_Reload_Ready_Backplate_Red->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
			m_pSpecial_Reload_Ready_Backplate->Set_Pos(_vec3{ m_tInfo.vPos.x - m_fSpecial_Reload_PlateX, m_tInfo.vPos.y + m_fSpecial_Reload_PlateY,0.f });
		}
	}

	m_pFlip_Red->Set_Pos(m_tInfo.vPos);
	m_pFlip_Glitch->Set_Pos(m_tInfo.vPos);
}

