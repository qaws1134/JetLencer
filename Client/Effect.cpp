#include "stdafx.h"
#include "Effect.h"
#include "GameObject_Manager.h"
#include "Prefab_Manager.h"
CEffect::CEffect()
	: m_bFrameStart(false)
	, m_fSize(0.f)
	, m_fReduce(0.f)
	, m_eEffectType(EFFECT::END)
	, m_bLoop(false)
	, m_fDelTime(0.f)
	, m_fReduceTime(0.f)
	, m_fReduceDelay(0.f)
	, m_bFrameEndNoDead(false)
	, m_bRed(false)
	, m_bStart(false)
	, m_bEnd(false)
{
}


CEffect::~CEffect()
{
	Release_GameObject();
}

CGameObject * CEffect::Create(const ANIMATION * _tAnimationInfo, D3DXVECTOR3 _vPos)
{
	CGameObject* pInstance = new CEffect;
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_Prefab(_tAnimationInfo);
	static_cast<CEffect*>(pInstance)->Set_Frame(_tAnimationInfo);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
CGameObject * CEffect::Create(EFFECT::TYPE _eEffectType, _vec3 _vPos)
{
	CGameObject* pInstance = new CEffect;
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_EffectType(_eEffectType);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
CGameObject * CEffect::Create(EFFECT::TYPE _eEffectType)
{
	CGameObject* pInstance = new CEffect;
	static_cast<CEffect*>(pInstance)->Set_EffectType(_eEffectType);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
CGameObject * CEffect::Create(EFFECT::TYPE _eEffectType,D3DXVECTOR3 _vPos, bool _bFrameStart)
{
	CGameObject* pInstance = new CEffect;
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_EffectType(_eEffectType);
	static_cast<CEffect*>(pInstance)->Set_FrameStart(_bFrameStart);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}
CGameObject * CEffect::Create(EFFECT::TYPE _eEffectType, D3DXVECTOR3 _vPos, bool _bFrameStart,float _fSize)
{
	CGameObject* pInstance = new CEffect;
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_EffectType(_eEffectType);
	static_cast<CEffect*>(pInstance)->Set_FrameStart(_bFrameStart);
	static_cast<CEffect*>(pInstance)->Set_Size(_fSize);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}


CGameObject * CEffect::Create(const PLACEMENT * _tPlacementInfo, D3DXVECTOR3 _vPos)
{
	CGameObject* pInstance = new CEffect;
	pInstance->Set_CenterPos(_tPlacementInfo->m_tMatInfo.mat[MATID::TRANS]);
	pInstance->Set_Pos(_vPos);
	static_cast<CEffect*>(pInstance)->Set_Prefab(_tPlacementInfo);
	static_cast<CEffect*>(pInstance)->Set_Frame(_tPlacementInfo);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}


HRESULT CEffect::Ready_GameObject()
{
	m_eRenderId = RENDERID::EFFECT;
	InitEffect();
	return S_OK;
}

int CEffect::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	Set_Texture();
	State_Change();
	return OBJ_NOEVENT;
}

void CEffect::Late_Update_GameObject()
{
}

void CEffect::Render_GameObject()
{
	WriteMatrix();
}

void CEffect::Release_GameObject()
{
}

void CEffect::Set_Frame(const ANIMATION * _tAnimationInfo)
{
	m_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	m_tFrame.fStartFrame = 0;
	m_tFrame.fMaxFrame = (float)m_pAnimation->iMax_Index;
}
void CEffect::Set_Frame(FRAME& _tFrame)
{
	_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	_tFrame.fStartFrame = 0;
	_tFrame.fMaxFrame = (float)m_pAnimation->iMax_Index;
}

void CEffect::Set_Frame(const PLACEMENT * _tPlacementInfo)
{
	m_pObjectInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(_tPlacementInfo->wstrObjName);
	m_pAnimation  = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(m_pObjectInfo->wstrIdleImage_ObjectKey+ m_pObjectInfo->wstrIdleImage_StateKey);

	m_tFrame.wstrObjKey = m_pAnimation->wstrObjectKey;
	m_tFrame.wstrStateKey = m_pAnimation->wstrStateKey;
	m_tFrame.fStartFrame = 0;
	m_tFrame.fMaxFrame = (float)m_pAnimation->iMax_Index;
	m_tFrame.fFrameSpeed = 30.f;
}

void CEffect::State_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	switch (m_eEffectType)
	{
	case EFFECT::ROCKET_PTFIRE:
		Size_Reduce();
		break;
	case EFFECT::ROCKET_BOOM_PTFIRE:
		Frame_Change();
		break;
	case EFFECT::JET_PTFIRE:
		Size_Reduce();
		break;
	case EFFECT::JET_PTFIRE_RED:
		Frame_Change();
		break;
	case EFFECT::CHAGE_BEAM:
		if (m_bFrameStart)
		{
			if (m_tInfo.vSize.x >= 0.8f)
				m_fReduce = 0.f;
			m_tInfo.vSize -= _vec3{ m_fReduce,m_fReduce,0.f };
			Frame_Change();
		}
		else
		{
			m_tInfo.vSize = { m_fSize,m_fSize,0.f };
			m_fReduce = -0.005f;
		}
		break;
	case EFFECT::GROUND_WATERSPLASH:
	case EFFECT::GROUND_WATERSPLASH_FAST:
		if (m_pTexInfo)
		{
			m_bCenter = true;
			m_fCenterY = float(m_pTexInfo->tImageInfo.Height);
			m_fCenterX = float(m_pTexInfo->tImageInfo.Width >> 1);
			if (m_tFrame.fFrameSpeed > 10.f)
			{
				m_tFrame.fFrameSpeed -= 0.02f;
			}
		}
		Frame_Change();
		break;
	case EFFECT::PLAYER:
		m_fDelTime += fTime;
		if (m_fDelTime >= 0.1f)
			m_bDead = true;
		break;
	case EFFECT::BOSS_LASER_END:
		if (m_bFrameStart)
			Frame_Change();
		break;
	case EFFECT::BOSS_CHAGE_BEAM:
		if (m_bFrameStart)
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			if (fTime != 0)
			{
				if (m_tInfo.vSize.x > 1.5f)
				{
					m_fReduceTime += fTime;
					if (m_fReduceTime > m_fReduceDelay)
						m_fReduce = 0.04f;
					else
						m_fReduce = 0.f;
				}

				m_tInfo.vSize -= _vec3{ m_fReduce,m_fReduce,0.f };
				if (m_tInfo.vSize.y < 0.f)
					m_tInfo.vSize = _vec3{ 0.f,0.f,0.f };
				Frame_Change();
			}
		}
		else
		{
			float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
			if (fTime != 0)
			{
				if (m_tInfo.vSize.x <= 0.f)
				{
					m_fSize = 0.f;
					m_fReduce = -0.01f;
				}
				m_tInfo.vSize = { m_fSize,m_fSize,0.f };

			Frame_Change();
			}
		}
		if ((int)m_tFrame.fStartFrame == 1)
		{
			m_tFrame.fStartFrame = 2.f;
		}
		break;
	case EFFECT::PLAYER_HIT_VFX:
		if (m_bFrameStart)
		{
			m_tInfo.vSize = { 3.f,3.f,0.f };

			m_tInfo.vPos = m_pTarget->Get_ObjInfo().vPos;

			Frame_Change();

		}
		else
		{
			m_tInfo.vSize = { 0.f,0.f,0.f };
			m_tFrame.fStartFrame = 0.f;
		}
		break;
	case EFFECT::MARKER_ROCKET_OVERLAY:
		if (m_bFrameStart)
		{
			m_tInfo.vSize = { 1.3f,1.3f,0.f };
			Frame_Change();
		}
		else
		{
			m_tInfo.vSize = { 0.f,0.f,0.f };
			m_tFrame.fStartFrame = 0.f;
		}
		break;
	case EFFECT::SNIPER_ARROW:
		if (m_bFrameStart)
		{
			if (!m_bStart)
			{
				m_tInfo.vSize = { 0.f,0.75f,0.f };
				m_bStart = true;
			}
			m_fReduceTime += fTime;
			if (m_fReduceTime > 1.f)
			{
				if(m_tInfo.vSize.x < 1.f)
					m_tInfo.vSize += _vec3{ 0.01f,0.f,0.f };
				else if (m_tInfo.vSize.x >= 1.f)
				{
					m_tInfo.vSize -= _vec3{ 0.f,0.0075f,0.f };
					if (m_tInfo.vSize.y <= 0.f)
					{
						m_bFrameStart = false;
						m_bEnd = true;
					}
				}
			}
		}
		else
		{
			m_tInfo.vSize = { 0.f,0.f,0.f };
			m_bStart = false;
		}
		break;
	case EFFECT::AFTERBURN_EFF:
		Frame_Change();
		m_tInfo.vPos.x -= fTime*float(rand()%200+800);
		break;
	case EFFECT::BOSS_DEAD:
		if (m_bFrameStart)
		{
			if (m_bEnd)
			{
				m_fReduce += 0.02f;
			}
			if (!m_bStart)
			{
				m_tInfo.vSize += _vec3{ m_fReduce,m_fReduce,0.f };
				if (m_tInfo.vSize.x > 10.f)
					m_bStart = true;
			}
			else
			{
				m_tInfo.vSize -= _vec3{ m_fReduce,m_fReduce,0.f };
				if (m_tInfo.vSize.x <= 0.f)
				{
					if (m_bEnd)
					{
						CGameObject_Manager::Get_Instance()->Set_GameEnd();
					}

					m_bFrameStart = false;
					m_bEnd = true;
					m_fReduce = 0.f;
				}
			}
		}
		break;
	default:
		Frame_Change();
		break;
	}
	
}


void CEffect::Size_Reduce()
{
	m_tInfo.vSize -= _vec3{ m_fReduce,m_fReduce,0.f };
	if (m_tInfo.vSize.x <= 0.f)
		m_bDead = true;
	m_tFrame.fStartFrame = 0;
}


void CEffect::Frame_Change()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_tFrame.fStartFrame += m_tFrame.fFrameSpeed*fTime;
	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
	{
		if (m_bLoop)
			m_tFrame.fStartFrame = 0.f;
		else
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
			if(!m_bFrameEndNoDead)
				m_bDead = true;
			else
				m_bFrameStart = false;
		}
	}
	else if (m_tFrame.fStartFrame <= 0.f)
	{
		if (m_bLoop)
			m_tFrame.fStartFrame = 0.f;
		else
		{
			m_tFrame.fStartFrame = m_tFrame.fMaxFrame - 1;
			if (!m_bFrameEndNoDead)
				m_bDead = true;
			else
				m_bFrameStart = false;
		}
	}
} 
void CEffect::InitEffect()
{
	switch (m_eEffectType)
	{
	case EFFECT::ROCKET_PTFIRE:
		m_pAnimation = 	CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire");
		Set_Frame(m_pAnimation);
		m_fSize = 0.15f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_fReduce = 0.01f;
		break;
	case EFFECT::ROCKET_BOOM_PTFIRE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire_Black");
		Set_Frame(m_pAnimation);
		m_tInfo.vSize = { m_fSize+ float(rand() % 10)*0.05f ,m_fSize+ float(rand() % 10)*0.05f,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		m_fAngle = (float)(rand() % 360);
		m_fReduce = 0.f;
		break;
	case EFFECT::ROCKET_BOOM_PTFIRE_RED :
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire");
		Set_Frame(m_pAnimation);
		m_tInfo.vSize = { m_fSize + float(rand() % 10)*0.05f ,m_fSize + float(rand() % 10)*0.05f,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		m_fAngle = (float)(rand() % 360);
		m_fReduce = 0.f;
		break;
	case EFFECT::JET_PTFIRE:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire");
		Set_Frame(m_pAnimation);

		m_fSize = float((rand() % 6) + 4) * 0.05f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_fReduce = float((rand() % 10) + 6) * 0.001f;
		break;
	case EFFECT::JET_PTFIRE_RED:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectPtfire");
		Set_Frame(m_pAnimation);

		m_fSize = float((rand() % 6) + 4) * 0.1f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = float(rand()%20+10);
		//m_fReduce = float((rand() % 10) + 6) * 0.001f;
		break;
	case EFFECT::CHAGE_BEAM:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"PlayerBeam_Charge");
		Set_Frame(m_pAnimation);
		m_fSize = 0.f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 50.f;
		m_fReduce = -0.005f;
		m_bLoop = true;
		m_tColor = { 200,255,255,255 };
		break;
	case EFFECT::GROUND_WATERSPLASH:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectWatersplash");
		Set_Frame(m_pAnimation);
		m_fSize = float(rand() %5+7)*0.1f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = float(rand() % 30 + 10);
		break;
	case EFFECT::GROUND_WATERSPLASH_FAST:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectWatersplash_fast");
		Set_Frame(m_pAnimation);
		m_fSize += float(rand() % 5 +1)*0.1f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = float(rand() % 20 + 20);
		break;
	case EFFECT::PLAYER:
		m_tFrame = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Frame();
		m_tInfo.vSize = CGameObject_Manager::Get_Instance()->Get_Player()->Get_ObjInfo().vSize;
		m_fAngle = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Angle();
		m_tColor.iAlpha = 150;
		break;
	case EFFECT::BOSS_LASER_END:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectLaser_end");
		Set_Frame(m_pAnimation);
		Set_Texture();
		m_fSize = 1.3f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		m_bLoop = true;
		m_bCenter = true;
		m_fCenterX = 0;
		m_fCenterY = float(m_pTexInfo->tImageInfo.Height>>1);
		break;
	case EFFECT::BOSS_CHAGE_BEAM:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"PlayerBeam_Charge");
		Set_Frame(m_pAnimation);
		m_fSize = 0.f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 50.f;
		m_fReduce = -0.01f;
		m_fReduceDelay = 3.f;
		m_bLoop = true;
		m_tColor = { 255,255,255,255 };
		break;
	case EFFECT::BULLET_IMPACT:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectBullet_impact");
		Set_Frame(m_pAnimation);
		m_fSize = float((rand() % 20+20)*0.025f);
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::OBJECT_IMPACT:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectBullet_impact");
		Set_Frame(m_pAnimation);
		m_fSize = float((rand() % 10 + 20)*0.2f);
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::BOOM_EXPLOSION:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectBoom_explosion");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::EXPLOSION1:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectExplosion1");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::EXPLOSION2:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectExplosion2");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::EXPLOSION3:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectExplosion3");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::MEGA_EXPLOSION1:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectMegaexplosion1");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::MEGA_EXPLOSION2:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectMegaexplosion2");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::MEGA_EXPLOSION3:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectMegaexplosion3");
		Set_Frame(m_pAnimation);
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::FIRE_BOSS:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectFire_boss");
		Set_Frame(m_pAnimation);
		m_fSize = float((rand() % 6) + 4) * 0.15f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_fReduce = float((rand() % 10) + 6) * 0.01f;
		m_tFrame.fFrameSpeed = 30.f;
		break;
	case EFFECT::PLAYER_HIT_VFX:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"PlayerHit_vfx");
		Set_Frame(m_pAnimation);
		m_fSize = 1.3f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 30.f;
		m_tFrame.fStartFrame = 0.f;
		m_bColor = true;
		m_bFrameEndNoDead = true;
		break;
	case EFFECT::MARKER_ROCKET_OVERLAY:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"GuiMarker_rocket_overlay");
		Set_Frame(m_pAnimation);
		m_fSize = 1.2f;
		m_tInfo.vSize = { m_fSize,m_fSize,0.f };
		m_tFrame.fFrameSpeed = 10.f;
		m_bLoop = true;
		break;
	case EFFECT:: SNIPER_ARROW:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EnemySniperArrow");
		Set_Frame(m_pAnimation);
		Set_Texture();
		m_bCenter = true;
		m_fCenterX = 0.f;
		m_fCenterY = float(m_pTexInfo->tImageInfo.Height>>1);
		m_tColor = { 150,255,255,255 };
		break;
	case EFFECT::AFTERBURN_EFF:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"Playerbrn_start_eff");
		Set_Frame(m_pAnimation);
		m_tInfo.vSize = { float((rand()%5+10)*0.1f),float((rand() % 5 + 10)*0.1f),0.f };
		Set_Texture();
		m_tFrame.fFrameSpeed =float(rand()%15 + 5);
		//m_bLoop = true;
		break;
	case EFFECT::BOSS_DEAD:
		m_pAnimation = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(L"EffectBoss_Dead");
		Set_Frame(m_pAnimation);
		m_tInfo.vSize = { 0.f,0.f,0.f };
		m_fReduce = 0.08f;
		m_bColor = true;
		Set_Texture();
		break;
	default:
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_fReduce = 0.f;
		m_tFrame.fFrameSpeed = 25.f;
		break;
	}
	Set_Texture();
}
