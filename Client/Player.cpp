#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Time_Manager.h"
#include "Spawn_Manager.h"
#include "Burner.h"
#include "ColSphere.h"
#include "Effect.h"

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
	m_fAccel = 3000.f;
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


	//회피
	m_fRollTime = 0.f;
	m_fRollCoolSpeed = 0.15f;
	m_eState = PLAYER::IDLE;


	//무기상태
	wstrSubWeapon = L"Rocket";		// Rocket,bottle , HommingRocket,
	wstrChargeWeapon = L"Beam";		// Beam, GunDrone, MultiHomming,boom, 


	if (wstrChargeWeapon == L"Beam")
	{
		m_pChargeBeam = CEffect::Create(EFFECT::CHAGE_BEAM,m_tInfo.vPos,false);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pChargeBeam);
	}

	//버너 생성

	ptFire = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire");
	m_pBurner = CBurner::Create();
	m_eBurnerState = BURNER::IDLE;	//START로 바꿔주자
	m_iMax_ptFireNum = 10;

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pBurner);

	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this,m_tCombatInfo,10.f, COLLIDER::PLAYER));

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
	Roll();					//상태 체크 후 ROLL 수행
	Keybord_OffSet();		//키보드를 통한 스크롤값 조정

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	
	FAILED_CHECK_RETURN(Set_Texture(), );
}

void CPlayer::Render_GameObject()
{
	WriteMatrix();
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
			break;
		case PLAYER::ROLL:
			m_tFrame.fStartFrame = 0.f;
			m_bRoll = true;
			break;
		}
	}
	m_ePreState = m_eState;
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
				m_eState = PLAYER::IDLE;
				m_bRoll = false;
			}
		}
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
		m_eBurnerState = BURNER::ACCEL;
		static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
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

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_bAuto)
	{
		if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_SPACE))
		{
			//쉐이크 효과

			m_fMaxSpeed = 800.f;

			Accel(m_tInfo.vDir, m_fAccel + m_fBoostAccel, m_fMaxSpeed, false);

			if (m_bMega)
				m_eBurnerState = BURNER::MEGA;
			else
				m_eBurnerState = BURNER::BURST;

			if (!m_bAccel)
			{
				if (m_bMega)
					static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::MEGA);
				else
					static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::BURST);
			}

			m_bAccel = true;
		}
		else if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_W))
		{
			m_fMaxSpeed = 600.f;
			Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);

			m_eBurnerState = BURNER::ACCEL;
			static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
			m_bAccel = true;
		}
		else
		{
			m_fMaxSpeed = 500.f;
			Accel(m_vGravity, m_fGravity, m_fMaxSpeed, true);
			m_eBurnerState = BURNER::IDLE;
			static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::IDLE);
			m_bAccel = false;
		}
	}


	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RBUTTON))
		m_eState = PLAYER::ROLL;
	

	SpawnPtFire();
	m_pBurner->Set_Pos(m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.8f));
	m_pBurner->Set_Angle(m_fAngle);

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LBUTTON) && AttackTime())
		CSpawn_Manager::Spawn(L"Minigun", m_tInfo.vPos- m_vVelocity*fTime, m_fAngle,m_vVelocity);

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
			m_fRocketTime[i] = 0.f;
			return true;
		}
	}
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

void CPlayer::TimeCheck()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fAttackTime += fTime;

	for (int i = 0; i < m_iMaxRocket; i++)
	{
		if (m_fRocketTime[i]<m_fRocketSpeed)
			m_fRocketTime[i] += fTime;
	}
	if (m_fChargeCoolTime < m_fChargeSpeed)
		m_fChargeCoolTime += fTime;
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
		CSpawn_Manager::Spawn(wstrSubWeapon, m_tInfo.vPos- m_tInfo.vDir*fTime+m_vVelocity*fTime, m_fAngle, m_vVelocity);
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
		break;
	}

}


void CPlayer::SubWeapon_Charge()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (ChargeShotTime())
	{
		//요깅
		//차지 이펙트 넣어주자
		if (wstrChargeWeapon == L"Beam")
		{
			if (m_fChargeTime > SubWeaponDelay)
			{
				static_cast<CEffect*>(m_pChargeBeam)->Set_FrameStart(true);
				m_pChargeBeam->Set_Pos(m_tInfo.vPos + m_tInfo.vDir*1000.f*fTime);
			}
		}
		if (m_fChargeTime < 6.f)
			m_fChargeTime += fTime;
	}
}

