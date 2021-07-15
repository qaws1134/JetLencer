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
{
}


CEffect::~CEffect()
{
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

	return OBJ_NOEVENT;
}

void CEffect::Late_Update_GameObject()
{
	FAILED_CHECK_RETURN(Set_Texture(), );
	State_Change();
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
		if (m_bFrameStart)
			Frame_Change();
		else
			Size_Reduce();
		break;
	case EFFECT::CHAGE_BEAM:
		if (m_bFrameStart)
		{
			if (m_tInfo.vSize.x > 0.8f)
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
	default:
		Frame_Change();
		break;
	}
	
}


void CEffect::Size_Reduce()
{
	m_tInfo.vSize -= _vec3{ m_fReduce,m_fReduce,0.f };
	if (m_tInfo.vSize.x < 0.f)
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
			m_bDead = true;
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
		m_tFrame.fFrameSpeed = 20.f;
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
	default:
		m_fSize = 1.f;
		m_tInfo.vSize = { m_fSize ,m_fSize,0.f };
		m_fReduce = 0.f;
		m_tFrame.fFrameSpeed = 30.f;
		break;
	}
}
