#include "stdafx.h"
#include "Player.h"
#include "Prefab_Manager.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Time_Manager.h"
#include "Spawn_Manager.h"
#include "Burner.h"
CPlayer::CPlayer()
	: m_fAttackTime(0.f)
	, m_fAttackSpeed(0.f)
	, m_fRocketSpeed(0.f)
	, m_eState(PLAYER::END)
	, m_ePreState(PLAYER::END)
	, m_bMega(false)
	, m_bAccel(false)
	, m_fRollTime(0.f)
	, m_fRollCoolSpeed(0.f)
{
	
}


CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Ready_GameObject()
{
	//m_tInfo
	m_tInfo.vPos.x = 400.f;
	m_tInfo.vPos.y = 300.f;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = D3DXVECTOR3(1.3f, 1.3f, 0.f);

	//프레임
	m_tFrame.wstrObjKey = L"Player";
	m_tFrame.wstrStateKey = L"Idle";
	m_tFrame.fMaxFrame = 16;
	m_tFrame.fFrameSpeed = 40.f;
	m_eRenderId = RENDERID::OBJECT;

	//이동
	m_vVelocity = { 1.f,1.f,0.f };
	m_vGravity = { 0.f,1.f,0.f };
	m_fRegistPower = 2000.f;
	m_fAngleSpeed = 200.f;
	m_fAccel = 1500.f;

	m_fAttackTime = 5.f;
	m_fAttackSpeed = 0.08f;

	m_fRocketSpeed = 5.f;

	m_fRollTime = 0.f;
	m_fRollCoolSpeed = 0.15f;

	for (int i = 0; i < 4; i++)
		m_fRocketTime[i] = m_fRocketSpeed;

	m_eState = PLAYER::IDLE;

	m_pBurner = CBurner::Create();
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pBurner);
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

	Set_Target(CGameObject_Manager::Get_Instance()->Get_Mouse());
	TargetAngle_Check();
	Key_State();
	State_Change();
	Frame_Change();
	Keybord_OffSet();

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

void CPlayer::State_Change()
{
	if (m_eState != m_ePreState)
	{
		switch (m_eState)
		{
		case PLAYER::IDLE:
			PlayerAngle_Check();
			break;
		case PLAYER::ROLL:
			m_tFrame.fStartFrame = 0.f;
			m_bRoll = true;
			break;
		}
	}
	m_ePreState = m_eState;
}

void CPlayer::Frame_Change()
{
	if (!m_bRoll)
	{
		m_eState = PLAYER::IDLE;
		PlayerAngle_Check();
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

void CPlayer::PlayerAngle_Check()
{
	for (float i = 0; i < 16; i++)
	{
		if (m_fAngle <= 12.5 + (i*22.5f))
		{
			m_tFrame.fStartFrame = i;
			return;
		}
	}
}

void CPlayer::Key_State()
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (!m_pTexInfo)
		return;
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_SPACE))
	{
		//쉐이크 효과, 부스터 이미지 생성 및 출력 
		Accel(m_tInfo.vDir,m_fAccel + 300.f, 800.f,false);
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
		Accel(m_tInfo.vDir,m_fAccel, 600.f,false);
		if (!m_bAccel)
			static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::ACCEL);
		m_bAccel = true;
	}
	else
	{
		Accel(m_vGravity, 200.f, 500.f, true);
		static_cast<CBurner*>(m_pBurner)->Set_BurnerState(BURNER::IDLE);
		m_bAccel = false;
	}

	if (0 < D3DXVec3Dot(&m_vVelocity, &m_tInfo.vDir))
		m_bMega = false;
	else
		m_bMega = true;
	m_pBurner->Set_Pos(m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.8f));
	m_pBurner->Set_Angle(m_fAngle);

	_vec3 vNormalVel = m_vVelocity;
	D3DXVec3Normalize(&vNormalVel, &vNormalVel);

	if (m_bAccel)
	{
		for (int i = 0; i < 10; i++)
		{
			CSpawn_Manager::Spawn(L"EffectPtfire", m_tInfo.vPos - m_tInfo.vDir*((float)(m_pTexInfo->tImageInfo.Width)*0.8f) - (vNormalVel*((float)i)), false, EFFECT::JET_PTFIRE);
		}
	}

	TimeCheck();
	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_LBUTTON) && AttackTime())
		CSpawn_Manager::Spawn(L"Minigun", m_tInfo.vPos, m_fAngle, m_vVelocity);

	if (CKey_Manager::Get_Instance()->Key_Pressing(KEY_RBUTTON) && AttackTime())
	{
		m_eState = PLAYER::ROLL;
	}

	if (CKey_Manager::Get_Instance()->Key_Down(KEY_E) && RocketTime())
	{
		CSpawn_Manager::Spawn(L"Rocket", m_tInfo.vPos, m_fAngle, m_vVelocity);
	}
	if (CKey_Manager::Get_Instance()->Key_Up(KEY_E) && ChargeShotTime())
	{

	}

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


bool CPlayer::AttackTime()
{
	if (m_fAttackTime > m_fAttackSpeed)
	{
		m_fAttackTime = 0.f;
		return true;
	}
	return false;
	
}

bool CPlayer::RocketTime()
{
	for (int i = 0; i < 4; i++)
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
	if (m_fAttackTime > m_fAttackSpeed)
	{
		m_fAttackTime = 0.f;
		return true;
	}
	return false;
}


void CPlayer::TimeCheck()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fAttackTime += fTime;
	for (int i = 0; i < 4; i++)
	{
		if (m_fRocketTime[i]<m_fRocketSpeed)
			m_fRocketTime[i] += fTime;
	}
}



void CPlayer::Accel(_vec3 vDir,float _fAccel, float _fMaxSpeed,bool bGravity)
{

	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	_vec3 vVec = m_vVelocity + vDir*_fAccel*fTime;
	float fVelLength = D3DXVec3Length(&vVec);

	if(bGravity)
		m_vVelocity += vDir*_fAccel*fTime;

	if (fVelLength > _fMaxSpeed + 30.f)
	{
		float fVelLength = D3DXVec3Length(&m_vVelocity);
		float fRegist = m_fRegistPower * fTime;
		if (fRegist > fVelLength)
			fRegist = fVelLength;
		_vec3 vRegist = (m_vVelocity* -fRegist);
		if (bGravity)
			vRegist.y = 0;
		D3DXVec3Normalize(&vRegist, &vRegist);
		m_vVelocity += vRegist*10.f;
		//최대속도 
	}
	else if (fVelLength > _fMaxSpeed)
		m_vVelocity *= _fMaxSpeed / fVelLength;
	else 
	{
		if (!bGravity)
			m_vVelocity += vDir*_fAccel*fTime;
	}

	m_tInfo.vPos += (m_vVelocity*fTime);
}

void CPlayer::DeadEffect()
{
	CSpawn_Manager::Spawn(L"Dead_Explosion2", m_tInfo.vPos);
}

