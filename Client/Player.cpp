#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Time_Manager.h"
#include "Spawn_Manager.h"
#include "Burner.h"
#include "ColSphere.h"
#include "ColRect.h"
#include "Effect.h"
#include "Flip.h"
#include "Rocket_Ui.h"
#include "Hp.h"
#include "DmgGrid.h"
#include "Danger.h"



#define SubWeaponDelay 1.f
#define ChargeWeaponDelayLV1 2.5f
#define ChargeWeaponDelayLV2 5.9f

CPlayer::CPlayer()
	: m_fRocketSpeed(0.f)
	, m_eState(PLAYER::END)
	, m_ePreState(PLAYER::END)
	, m_bMega(false)
	, m_fRollTime(0.f)
	, m_fRollCoolSpeed(0.f)
	, m_fChargeTime(0.f)
	, m_fChargeSpeed(0.f)
	, m_fChargeCoolTime(0.f)
	, wstrSubWeapon(L"")
	, wstrChargeWeapon(L"")
	, m_pGuiLFlip(nullptr)
	, m_pGuiRFlip(nullptr)
	, m_pGuiDamageGrid(nullptr)
	, m_pGuiHp(nullptr)
	, m_fAfterBurnTime(0.f)
	, m_fAfterBurnlimit(0.f)
	, m_bOverHeat(false)
	, m_fReduceAccelRate(0.f)
	, m_bZoom(false)
	,m_fSuperTime(0.f)
	,m_fSuperSpeed(0.f)
	, m_fSpectrumTime(0.f)
	, m_bSpectrum(false)
	, m_bSuperEvade(false)
	, m_eDangerState(DANGER::END)
	, m_ePreDangerState(DANGER::END)
	, m_iRocketNum(0)
{
	
}


CPlayer::~CPlayer()
{

}

CGameObject * CPlayer::Create()
{
	CGameObject* pInstance = new CPlayer;
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}


HRESULT CPlayer::Ready_GameObject()
{
	
	//m_tInfo
	m_tInfo.vPos.x = float(Map_Width>>1);
	m_tInfo.vPos.y = float(Map_Height>>1);
	m_tInfo.vSize = D3DXVECTOR3(1.3f, 1.3f, 0.f);


	//체력,공격력
	m_tCombatInfo.iHp = 6;

	//프레임
	//플레이어 오브젝트 생성 후 삭제 ㄱ
	m_tFrame.wstrObjKey = L"Player";
	m_tFrame.wstrStateKey = L"Idle";
	m_tFrame.fMaxFrame = 16;
	m_tFrame.fFrameSpeed = 40.f;	//롤 스피드


	m_eRenderId = RENDERID::OBJECT;

	//이동
	m_vVelocity = { 1.f,1.f,0.f };
	m_vGravity = { 0.f,1.f,0.f };
	m_fRegistPower = 2000.f;
	m_fAngleSpeed = 220.f;
	m_fAngle_per_Frame = 22.5f;
	m_fAngleOffset = 12.5f;
	m_fAccel = 2500.f;
	m_fBoostAccel = 400.f;
	m_fGravity = 200.f;

	//공격
	//일반공격
	m_fAttackTime = 5.f;
	m_fAttackSpeed = 0.08f;

	m_fMaxSpeedOffSet = 30.f;

	//로켓
	m_fRocketSpeed = 5.f;

	m_iMaxRocket = 4;
	for (int i = 0; i < m_iMaxRocket; i++)
		m_fRocketTime[i] = m_fRocketSpeed;

	//차지공격
	m_fChargeTime = 5.f;
	m_fChargeSpeed = 0.08f;
	m_eSubWeaponState = BULLET::CHARGE_0;
	m_fChargeCoolTime = 10.f;

	//회피
	m_fRollTime = 0.f;
	m_fRollCoolSpeed = 0.15f;
	m_eAfterBurnState = PLAYER::IDLE;

	m_fSuperSpeed = 1.f;
	m_fSuperTime = 1.f;

	m_fSpectrumTime = 1.f;

	//무기상태
	wstrSubWeapon = L"Rocket_alt";		// Rocket,bottle , HommingRocket,
	wstrChargeWeapon = L"Beam";		// Beam, GunDrone, MultiHomming,boom, 
	//Rocket_alt

	//빔 차지 이펙트
	if (wstrChargeWeapon == L"Beam")
	{
		m_pChargeBeam = CEffect::Create(EFFECT::CHAGE_BEAM,m_tInfo.vPos,false);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pChargeBeam);
	}

	m_pBurner = CBurner::Create();
	m_eBurnerState = BURNER::IDLE;	//START로 바꿔주자
	m_iMax_ptFireNum = 10;

	m_fAfterBurnTime = 0.f;
	m_fAfterBurnlimit = 4.f;
	m_fReduceAccelRate = 0.08f;

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pBurner);


	//컬라이더
#define PlayerColliderSize	10.f
	m_vecCollider.reserve(8);
	m_vecCollider.emplace_back(CColSphere::Create(this,m_tCombatInfo, PlayerColliderSize, COLLIDER::PLAYER));
	for (int i = 0; i < 4; i++)
		m_vecCollider.emplace_back(CColRect::Create(this, (float)WINCX*(i+1),(float)WINCY*(i+1), COLLIDER::PLAYER_SEARCH));
	m_vecCollider.emplace_back(CColSphere::Create(this, 100.f, COLLIDER::ROCKET_SEARCH));



	//Gui 생성

	_vec3 vLOffset = { 0.3f,0.7f,0.f };
	_vec3 vROffset = { 0.7f,0.7f,0.f };
	m_pGuiLFlip = CFlip::Create(_vec3{ float(WINCX)*vLOffset.x,float(WINCY)*vLOffset.y,0.f },false);
	m_pGuiRFlip = CFlip::Create(_vec3{ float(WINCX)*vROffset.x,float(WINCY)*vROffset.y,0.f }, true);

	m_pGuiRocket = CRocket_Ui::Create(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 160.f,0.f });

	m_pGuiHp = CHp::Create(_vec3{ float(WINCX >> 1),float(WINCY >> 1) + 200.f,0.f });
	static_cast<CHp*>(m_pGuiHp)->Set_Hp(m_tCombatInfo.iHp);
	m_pGuiDamageGrid = CDmgGrid::Create();

	m_pEffectHitVfx = CEffect::Create(EFFECT::PLAYER_HIT_VFX);
	m_pEffectHitVfx->Set_Target(this);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pEffectHitVfx);

	m_pGuiDanger = CDanger::Create(_vec3{float(WINCX>>1),float(WINCY >> 1)-250.f,0.f });


	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_Q))
	{
		DeadEffect();
	}
	if (m_bDead)
	{
		DeadEffect();
		return OBJ_DEAD;
	}


	m_pTarget = CGameObject_Manager::Get_Instance()->Get_Mouse();

	if (!m_pTexInfo)
		return OBJ_NOEVENT;
	
	PositionRock_Check();
	TargetAngle_Check();	//마우스와 각도 체크
	TimeCheck();			//공격속도,부스트 시간 체크
	Key_State();			//키 입력을통한 상태값 변경

	State_Change();			//ROLL 상태 체크
	static_cast<CHp*>(m_pGuiHp)->End_Super(SuperTime());
	Roll();					//상태 체크 후 ROLL 수행
	Keybord_OffSet();		//키보드를 통한 스크롤값 조정

	return OBJ_NOEVENT;
}


void CPlayer::Release_GameObject()
{
}


void CPlayer::State_Change()
{
	if (m_eState != m_ePreState)
	{
		switch (m_eState)
		{
		case PLAYER::IDLE:
			JetAngleCheck();
			static_cast<CHp*>(m_pGuiHp)->Set_State(m_eState);
			break;
		case PLAYER::ROLL:
			m_tFrame.fStartFrame = 0.f;
			m_fSuperSpeed = 1.f;
			m_fSuperTime = 0.f;
			m_bRoll = true;
			break;
		case PLAYER::STOP:
			CTime_Manager::Get_Instance()->Set_Stop(1.f);
			break;
		case PLAYER::EVEDE:
			m_fSpectrumTime = 0.f;
			m_bSpectrum = true;
			break;
		case PLAYER::SUPER_EVEDE:
			CScroll_Manager::Shake(5.f, 0.5f);
			static_cast<CDmgGrid*>(m_pGuiDamageGrid)->Set_Green(true);
			static_cast<CEffect*>(m_pEffectHitVfx)->Set_Red(false);
			static_cast<CEffect*>(m_pEffectHitVfx)->Set_FrameStart(true);
			m_fSuperSpeed = 1.f;
			m_fSuperTime = 0.f;
			m_bSuperEvade = false;
			break;
		case PLAYER::HIT:
			CScroll_Manager::Shake(5.f, 0.5f);
			m_fSuperTime = 0.f;	
			m_fSuperSpeed = 1.5f;
			m_tCombatInfo.iHp -= 1;
			static_cast<CHp*>(m_pGuiHp)->Set_State(m_eState);
			static_cast<CHp*>(m_pGuiHp)->Set_Hp(m_tCombatInfo.iHp);
			static_cast<CEffect*>(m_pEffectHitVfx)->Set_Red(true);
			static_cast<CEffect*>(m_pEffectHitVfx)->Set_FrameStart(true);

			if (m_tCombatInfo.iHp <= 0)
			{
				DeadEffect();
			}
			else
			{
				static_cast<CDmgGrid*>(m_pGuiDamageGrid)->Set_Red(true);
			}
			break;
		}
		m_ePreState = m_eState;
	}

	if (m_ePreAfterBurnState != m_eAfterBurnState)
	{
		switch (m_eAfterBurnState)
		{
		case PLAYER::IDLE:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_PlayerState(PLAYER::IDLE);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_PlayerState(PLAYER::IDLE);
			break;
		case PLAYER::ACCEL:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_PlayerState(PLAYER::ACCEL);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_PlayerState(PLAYER::ACCEL);
			break;
		case PLAYER::AFTER_BURNUR:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_PlayerState(PLAYER::AFTER_BURNUR);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_PlayerState(PLAYER::AFTER_BURNUR);
			break;
		case PLAYER::OVERHEAT:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_PlayerState(PLAYER::OVERHEAT);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_PlayerState(PLAYER::OVERHEAT);
			static_cast<CRocket_Ui*>(m_pGuiRocket)->Set_Red(true);
			static_cast<CHp*>(m_pGuiHp)->Set_Red(true);
			break;
		}
		m_ePreAfterBurnState = m_eAfterBurnState;
	}


	if (m_eWeaponState != m_ePreWeaponState)
	{
		switch (m_eWeaponState)
		{
		case PLAYER::SPECIAL_CHARGE:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_WeaponState(PLAYER::SPECIAL_CHARGE);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_WeaponState(PLAYER::SPECIAL_CHARGE);
			break;
		case PLAYER::SPECIAL_RELOAD_START:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_WeaponState(PLAYER::SPECIAL_RELOAD_START);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_WeaponState(PLAYER::SPECIAL_RELOAD_START);
			break;
		case PLAYER::SPECIAL_RELOAD_END:
			static_cast<CFlip*>(m_pGuiLFlip)->Set_WeaponState(PLAYER::SPECIAL_RELOAD_END);
			static_cast<CFlip*>(m_pGuiRFlip)->Set_WeaponState(PLAYER::SPECIAL_RELOAD_END);
			break;
		}
		m_ePreWeaponState = m_eWeaponState;
	}


	//if (m_ePreDangerState != m_eDangerState)
	//{
		switch (m_eDangerState)
		{
		case DANGER::DANGER_START:
			//처음 들어왔을 때 
			if (m_iRocketNum == 1)
				static_cast<CDanger*>(m_pGuiDanger)->Set_DangerState(DANGER::DANGER_START);
			else
				m_eDangerState = DANGER::DANGER_IDLE;
			break;
		case DANGER::DANGER_IDLE:
			static_cast<CDanger*>(m_pGuiDanger)->Set_DangerState(DANGER::DANGER_IDLE);

			break;
		case DANGER::DANGER_END:
			if (m_iRocketNum == 0)
			{
				static_cast<CDanger*>(m_pGuiDanger)->Set_DangerState(DANGER::DANGER_END);
				if (static_cast<CDanger*>(m_pGuiDanger)->Get_Danger_End())
				{
					m_eDangerState = DANGER::END;
				}
				break;
			}
		case DANGER::END:
			if (m_iRocketNum == 0)
			{
				static_cast<CDanger*>(m_pGuiDanger)->Set_DangerState(DANGER::END);
				break;
			}
		}
		m_ePreDangerState = m_eDangerState;
	//}
	if (m_ePreDangerState != DANGER::DANGER_END)
	{
		if (m_iRocketNum == 0)
		{
			static_cast<CDanger*>(m_pGuiDanger)->Set_DangerState(DANGER::END);
		}
	}

	static_cast<CDanger*>(m_pGuiDanger)->Set_FrameSpeed(m_fDangerLength);
}

void CPlayer::Roll()
{
	if (!m_bRoll)
	{
		m_eState = PLAYER::IDLE;
		JetAngleCheck();
	}
	else
	{
		float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
		m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
		if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
			m_fRollTime += fTime;

			if (m_fRollTime> m_fRollCoolSpeed)
			{
				m_fRollTime = 0.f;
				m_tFrame.fStartFrame = 0;
				m_bRoll = false;
			}
		}
		if (m_bSuperEvade)
			m_eState = PLAYER::SUPER_EVEDE;
	}
}



void CPlayer::PositionRock_Check()
{
	_vec3 vMouse = static_cast<CMouse*>(m_pTarget)->Get_MousePos();
	_vec3 vMouseResult = vMouse;
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();

	_vec3 NextPos = m_tInfo.vPos + m_vVelocity*50.f*fTime;

	if (0 > NextPos.x|| Map_Width<NextPos.x)
	{
		vMouseResult.x = WINCX >> 1;
		m_bAuto = true;
	}
	if (0 > NextPos.y||Map_Height<NextPos.y)
	{
		vMouseResult.y = WINCY >> 1;
		m_bAuto = true;
	}

	if (!CScroll_Manager::Get_ScrollLock(SCROLL::LEFT)
		&& !CScroll_Manager::Get_ScrollLock(SCROLL::TOP)
		&& !CScroll_Manager::Get_ScrollLock(SCROLL::RIGHT)
		&& !CScroll_Manager::Get_ScrollLock(SCROLL::BOTTOM))
		m_bAuto = false;

	m_vTarget_Dir = vMouseResult- m_tInfo.vPos;

	if (m_bAuto)
	{
		m_fMaxSpeed = 600.f;
		Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);

		if(!m_bOverHeat)
			m_eAfterBurnState = PLAYER::IDLE;
		m_eBurnerState = BURNER::ACCEL;
		static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
		m_fAfterBurnTime -= fTime;
		m_pGuiLFlip->Set_Pos(_vec3{ m_pGuiLFlip->Get_ObjInfo().vPos.x - m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiLFlip->Get_ObjInfo().vPos.y,0.f });
		m_pGuiRFlip->Set_Pos(_vec3{ m_pGuiRFlip->Get_ObjInfo().vPos.x + m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiRFlip->Get_ObjInfo().vPos.y,0.f });

		m_bAccel = true;
	}
}

void CPlayer::Key_State()
{
	if (!m_bAccel)
	{
		if (0 < D3DXVec3Dot(&m_tInfo.vDir, &m_vVelocity))
			m_bMega = false;
		else
			m_bMega = true;
	}
	
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_L))
	{
		m_eState = PLAYER::EVEDE;

	}
	if(m_bSpectrum)
		Spectrum();

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bAuto)
	{
		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_SPACE)&& !m_bOverHeat)
		{
			//쉐이크 효과

			m_fMaxSpeed = 800.f;

			Accel(m_tInfo.vDir, m_fAccel + m_fBoostAccel, m_fMaxSpeed, false);
			m_eAfterBurnState = PLAYER::AFTER_BURNUR;
			if (m_bMega)
			{
				m_eBurnerState = BURNER::MEGA;
			}
			else
			{
				m_eBurnerState = BURNER::BURST;
			}

			if (!m_bAccel)
			{
				if (m_bMega)
					static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::MEGA);
				else
					static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::BURST);
			}
			m_fAfterBurnTime += fTime;
			m_pGuiLFlip->Set_Pos(_vec3{ m_pGuiLFlip->Get_ObjInfo().vPos.x + m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiLFlip->Get_ObjInfo().vPos.y,0.f });
			m_pGuiRFlip->Set_Pos(_vec3{ m_pGuiRFlip->Get_ObjInfo().vPos.x - m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiRFlip->Get_ObjInfo().vPos.y,0.f });


			m_bAccel = true;
		}
		else if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_W))
		{
			m_fMaxSpeed = 600.f;
			Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
			m_eAfterBurnState = PLAYER::ACCEL;

			m_eBurnerState = BURNER::ACCEL;
			static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);

			m_fAfterBurnTime -= fTime;
			m_pGuiLFlip->Set_Pos(_vec3{ m_pGuiLFlip->Get_ObjInfo().vPos.x - m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiLFlip->Get_ObjInfo().vPos.y,0.f });
			m_pGuiRFlip->Set_Pos(_vec3{ m_pGuiRFlip->Get_ObjInfo().vPos.x + m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiRFlip->Get_ObjInfo().vPos.y,0.f });
			m_bAccel = true;
		}
		else
		{
			m_fMaxSpeed = 500.f;
			Accel(m_vGravity, m_fGravity, m_fMaxSpeed, true);
			if(!m_bOverHeat)
				m_eAfterBurnState = PLAYER::IDLE;

			m_eBurnerState = BURNER::IDLE;
			static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::IDLE);

			m_fAfterBurnTime -= fTime;
			m_pGuiLFlip->Set_Pos(_vec3{ m_pGuiLFlip->Get_ObjInfo().vPos.x - m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiLFlip->Get_ObjInfo().vPos.y,0.f });
			m_pGuiRFlip->Set_Pos(_vec3{ m_pGuiRFlip->Get_ObjInfo().vPos.x + m_fAfterBurnTime*m_fReduceAccelRate,m_pGuiRFlip->Get_ObjInfo().vPos.y,0.f });
			m_bAccel = false;
		}
	}


	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RBUTTON))
		m_eState = PLAYER::ROLL;

	SpawnPtFire();
	m_pBurner->Set_Pos(m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.8f));
	m_pBurner->Set_Angle(m_fAngle);

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LBUTTON) && AttackTime())
	{
		CSpawn_Manager::Spawn(L"Minigun", m_tInfo.vPos - m_vVelocity*fTime, m_fAngle, m_vVelocity);
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_E))
		m_fChargeTime = 0.f;
	
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_E) )
		SubWeapon_Charge();

	if (CKey_Manager::Get_Instance()->Key_Up(KEY_E)&& SubWeapon_Check())
		SubWeapon_Select();

}


void CPlayer::Keybord_OffSet()
{
	_vec3 vOffSet = { float(WINCX >> 1),float(WINCY >> 1),0.f };
	_vec3 vScroll = CScroll_Manager::Get_Scroll();

	if (vOffSet.x < m_tInfo.vPos.x + vScroll.x)
	{
		CScroll_Manager::Set_Scroll(vOffSet - (m_tInfo.vPos + vScroll));
	}
	if (vOffSet.x > m_tInfo.vPos.x + vScroll.x)
	{
		CScroll_Manager::Set_Scroll(vOffSet - (m_tInfo.vPos + vScroll));
	}
	
}



bool CPlayer::RocketTime()
{
	
	for (int i = 0; i < m_iMaxRocket; i++)
	{
		if (m_fRocketTime[i] >= m_fRocketSpeed)
		{
			static_cast<CRocket_Ui*>(m_pGuiRocket)->Start_Rocket(i);
			m_fRocketTime[i] = 0.f;
			return true;
		}
	}
	static_cast<CRocket_Ui*>(m_pGuiRocket)->UnReady();
	return false;
}

bool CPlayer::ChargeShotTime()
{
	if (m_fChargeCoolTime > m_fChargeSpeed)
	{
		return true;
	}
	return false;
}

bool CPlayer::SuperTime()
{
	if (m_fSuperTime < m_fSuperSpeed)
	{
		return true;
	}
	return false;
}

void CPlayer::TimeCheck()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fAttackTime += fTime;

	for (int i = 0; i < m_iMaxRocket; i++)
	{
		if (m_fRocketTime[i] < m_fRocketSpeed)
			m_fRocketTime[i] += fTime;
		else
			static_cast<CRocket_Ui*>(m_pGuiRocket)->End_Rocket(i);
	}
	//충전된 값을 스피드에 넘겨줘서 체크 
	if (m_fChargeCoolTime < m_fChargeSpeed)
	{
		m_fChargeCoolTime += fTime;
		if(m_fChargeCoolTime > m_fChargeSpeed)
			m_eWeaponState = PLAYER::SPECIAL_RELOAD_END;
	}
	if (m_fAfterBurnTime >= m_fAfterBurnlimit)
	{
  		m_bOverHeat = true;
		m_eAfterBurnState = PLAYER::OVERHEAT;
	}

	if (m_fAfterBurnTime < 0)
	{
		static_cast<CFlip*>(m_pGuiLFlip)->Set_OverHeat(false);
		static_cast<CFlip*>(m_pGuiRFlip)->Set_OverHeat(false);
		static_cast<CRocket_Ui*>(m_pGuiRocket)->Set_Red(false);
		static_cast<CHp*>(m_pGuiHp)->Set_Red(false);
		m_bOverHeat = false;
		m_fAfterBurnTime = 0.f;
	}
	
	static_cast<CHp*>(m_pGuiHp)->End_Super(SuperTime());
	if (m_fSuperTime < m_fSuperSpeed)
	{
		m_fSuperTime += fTime;
	}

}

void CPlayer::DeadEffect()
{
	CSpawn_Manager::Spawn(L"Dead_Explosion1", m_tInfo.vPos);
}

bool CPlayer::SubWeapon_Check()
{

	if (m_fChargeTime < SubWeaponDelay)
	{
		m_eSubWeaponState = BULLET::CHARGE_0;
		return RocketTime();
	}
	else if (m_fChargeTime < ChargeWeaponDelayLV1)
		m_eSubWeaponState = BULLET::CHARGE_1;
	else if (m_fChargeTime < ChargeWeaponDelayLV2)
		m_eSubWeaponState = BULLET::CHARGE_2;
	else 
		m_eSubWeaponState = BULLET::CHARGE_3;
	
	if (ChargeShotTime())
	{
		m_fChargeSpeed = m_fChargeTime;
		m_fChargeCoolTime = 0.f;
		return true;
	}
	return false;

}

void CPlayer::SubWeapon_Select()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	switch (m_eSubWeaponState)
	{
	case BULLET::CHARGE_0:
		CSpawn_Manager::Spawn(wstrSubWeapon, m_tInfo.vPos+ m_tInfo.vDir*fTime+m_vVelocity*2.f*fTime, m_fAngle, m_vVelocity);
		break;
	case BULLET::CHARGE_1: 
	case BULLET::CHARGE_2:
	case BULLET::CHARGE_3:
		CSpawn_Manager::Spawn(wstrChargeWeapon, m_tInfo.vPos- m_vVelocity*fTime, m_fAngle, m_vVelocity, m_eSubWeaponState);
		if (wstrChargeWeapon == L"Beam")
		{
			static_cast<CEffect*>(m_pChargeBeam)->Set_FrameStart(false);
			static_cast<CEffect*>(m_pChargeBeam)->Set_Size(0.f);
		}
		m_eWeaponState = PLAYER::SPECIAL_RELOAD_START;
		
		break;
	}

}


void CPlayer::SubWeapon_Charge()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (ChargeShotTime())
	{
		if (m_fChargeTime > SubWeaponDelay)
		{
			if (wstrChargeWeapon == L"Beam")
			{
				static_cast<CEffect*>(m_pChargeBeam)->Set_FrameStart(true);
				m_pChargeBeam->Set_Pos(m_tInfo.vPos + m_tInfo.vDir*1000.f*fTime);
			}
			m_eWeaponState = PLAYER::SPECIAL_CHARGE;
		}
		if (m_fChargeTime < 6.f)
			m_fChargeTime += fTime;
	}
}

void CPlayer::Spectrum()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);
	m_fSpectrumTime += fTime;
	m_fSpecAddNumTime += fTime;
	if (m_fSpecAddNumTime > 0.05f)
	{
		m_iSpectrum++;
		m_fSpecAddNumTime = 0.f;
		CSpawn_Manager::Spawn(EFFECT::PLAYER, m_tInfo.vPos
			- m_tInfo.vDir, false);
	}
	if (m_fSpectrumTime >1.0f)
	{
		m_bSpectrum = false;
		m_iSpectrum = 0;
	}
	
}


