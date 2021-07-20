#include "stdafx.h"
#include "Serpent.h"
#include "Prefab_Manager.h"
#include "SerpentObject.h"
#include "SerpentHead.h"
#include "SerpentBody.h"
#include "SerpentTail.h"
#include "ColSphere.h"
#include "Effect.h"
#include "Gui.h"
CSerpent::CSerpent()
	: m_bStart(false)
	, m_iMaxHp(0)
	, m_iMaxBody(0)
	, m_eSelected_Pattern(SERPENT::END)
	, m_pBoss_Hp(nullptr)
	,m_pBoss_Hp_Plate(nullptr)
	,m_pBoss_Hp_Red(nullptr)
	, m_bPattern_Start(false)
	, m_fPatternEnd(0.f)
	,m_fPatternTime(0.f)
	, m_bGravity(false)
	, m_iCircleIndex(0)
	, m_iCircleEndIdx(0)
	, m_bPage_Start(false)
	, m_bCircle_Rager(false)
	,m_fHp_RedTime(0.f)
	,m_fHp_RedSpeed(0.f)
	, m_iPreHp(0)
{
	m_vecSerpent.reserve(64);
}

CSerpent::~CSerpent()
{
}

CGameObject * CSerpent::Create(_vec3 vPos)
{
	CGameObject* pInstance = new CSerpent;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);
	pInstance->Set_Pos(vPos);
	if(FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}

HRESULT CSerpent::Ready_GameObject()
{

	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 25.f, COLLIDER::ENEMY));

	m_fAccel = 3000.f;
	m_fMaxSpeed = 500.f;
	m_fRegistPower = 2000.f;
	m_fGravity = 200.f;
	m_vVelocity = { 1.f,1.f,0.f };
	m_vGravity = { 0.2f,1.f,0.f };
	m_fAngleSpeed = 50.f;
	m_iMaxBody = 50;

	m_fHp_RedSpeed = 2.f;

	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;
	m_fPatternEnd = 10.f;
	pHead = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Serpent_Head");
	pBody = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Serpent_Body");
	pTail = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Serpent_Tail");

	m_pBoss_Hp		 = CGui::Create(UI::BOSS_HP);
	m_pBoss_Hp_Red	 = CGui::Create(UI::BOSS_HP_RED);
	m_pBoss_Hp_Plate = CGui::Create(UI::BOSS_HP_PLATE);



	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), m_pBoss_Hp_Plate);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), m_pBoss_Hp_Red);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), m_pBoss_Hp);



	Set_Texture();

	for (int i = 0; i < m_iMaxBody; i++)
	{
		CGameObject* pSerpentObject = nullptr;
		if (i == 0)
		{
			pSerpentObject = CSerpentHead::Create(pHead);
			m_vecSerpent.emplace_back(pSerpentObject);
		}
		else if (i == m_iMaxBody-1)
		{
			pSerpentObject = CSerpentTail::Create(pTail);
			m_vecSerpent.emplace_back(pSerpentObject);
		}
		else
		{
			pSerpentObject = CSerpentBody::Create(pBody);
			m_vecSerpent.emplace_back(pSerpentObject);
		}

		pSerpentObject = nullptr;
	}
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), this);

	m_pChargeBeam = CEffect::Create(EFFECT::BOSS_CHAGE_BEAM, m_tInfo.vPos, false);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::EFFECT), m_pChargeBeam);

	Init_Hp();
	Init_Pos();
	Init_Target();
	return S_OK;
}

int CSerpent::Update_GameObject()
{
	if (m_bDeadEffect)
		DeadEffect();

	//if (CKey_Manager::Get_Instance()->Key_Down(KEY_NUM4))
	//{
	//	m_eTailPattern = SERPENT::TAILSHOTGUN;
	//}

	//���� ����
	Hp_Stage();
	Select_Page();
	State_Change();
	Ai_State();

	//���Ͽ� ���� �̵� 
	return OBJ_NOEVENT;
}

void CSerpent::Late_Update_GameObject()
{
}

//���� �ʱ�ȭ
void CSerpent::State_Change()
{
	switch (m_eSerpentPagePattern)
	{
	case SERPENT::PAGE1:
		if (m_tInfo.vPos.y < m_pTarget->Get_ObjInfo().vPos.y + 100.f)
			m_bGravity = true;
		if (m_tInfo.vPos.y > GroundY + 1500)
			m_bGravity = false;
		break;
	case SERPENT::PAGE2:
		m_bGravity = false;
		if (!m_bPattern_Start)
		{
			switch (m_eSelected_Pattern)
			{
			case SERPENT::CIRCLE_BULLET:
				m_RandfX = float(rand() % 300);
				m_RandfY = float(rand() % 300);
				if (rand() % 2 == 0)
					m_RandfX *= -1;
				if (rand() % 2 == 0)
					m_RandfY *= -1;
				//if (m_tInfo.vPos.y + m_RandfY < GroundY - 50.f)
				//	m_eSelected_Pattern = SERPENT::CIRCLE_BULLET;

				m_vEnd_Dir = _vec3{ m_tInfo.vPos.x + m_RandfX,m_tInfo.vPos.y + m_RandfY,0.f };
				m_fAngleSpeed = 270.f;
				m_fAccel = 5000.f;
				m_fMaxSpeed = 700.f;
				m_fAttackSpeed = 0.1f;
				m_fAttackTime = 0.f;
				m_fPatternTime = 0.f;
				m_fPatternEnd = 3.f;
				m_iCircleIndex = 0;
				m_bPattern_Start = true;
				break;
			case SERPENT::RAGER:
				//m_eSelected_Pattern = SERPENT::RAGER;

				m_fAngleSpeed = 30.f;
				m_fAccel = 3000.f;
				m_fMaxSpeed = 200.f;
				m_fPatternTime = 0.f;
				m_fPatternEnd = 3.f;
				m_bPattern_Start = true;

				for (auto& iter : m_vecSerpent)
				{
					if (iter == m_vecSerpent.front())
					{
						static_cast<CSerpentHead*>(iter)->Set_Pattern(SERPENT::RAGER);
						static_cast<CEffect*>(m_pChargeBeam)->Set_FrameStart(true);
						static_cast<CEffect*>(m_pChargeBeam)->Set_Size(0.2f);
					}

					iter->Set_TrueMod(true);
				}
				break;
			case SERPENT::END:
				//m_eSelected_Pattern = SERPENT::END;
				m_fPatternTime = 0.f;
				m_fPatternEnd = 4.f;
				m_fAngleSpeed = 50.f;
				m_fAccel = 3000.f;
				m_fMaxSpeed = 350.f;
				m_bPattern_Start = true;
				
				break;

			}

			if (m_tCombatInfo.iHp < (int)(float(m_iMaxHp) *(40.f / 100.f)))
				m_eTailPattern = SERPENT::TAILSHOTGUN;
			else
				m_eTailPattern = SERPENT::TAILGUN;
			break;
		}

		switch (m_eTailPattern)
		{
		case SERPENT::RAGER:
			static_cast<CSerpentObject*>(m_vecSerpent.back())->Set_Pattern(SERPENT::RAGER);
			break;
		case SERPENT::TAILGUN:
			static_cast<CSerpentObject*>(m_vecSerpent.back())->Set_Pattern(SERPENT::TAILGUN);
			break;
		case SERPENT::TAILSHOTGUN:
			static_cast<CSerpentObject*>(m_vecSerpent.back())->Set_Pattern(SERPENT::TAILSHOTGUN);
			break;
		}
		break;
	}
}
void CSerpent::DeadEffect()
{

}
void CSerpent::Hp_Stage()
{
	int iInitHp = 0;
	for (auto& iter : m_vecSerpent)
	{
		if (!static_cast<CSerpentObject*>(iter)->Get_Crash())
			iInitHp += iter->Get_ColVec(0)->Get_CombatInfo().iHp;

	}
	m_tCombatInfo.iHp = iInitHp;
	float fHpRatio = ((float)m_tCombatInfo.iHp / (float)m_iMaxHp);
	static_cast<CGui*>(m_pBoss_Hp)->Set_HpSize(fHpRatio);
	if (m_iPreHp != m_tCombatInfo.iHp)
	{
		m_fHp_RedTime = 0.f;
	}
	m_iPreHp = m_tCombatInfo.iHp;

	if (Hp_RedTime())
	{
		static_cast<CGui*>(m_pBoss_Hp_Red)->Set_HpSize(fHpRatio);
	}
}

//���� ����
void CSerpent::Select_Page()
{

	if (m_tCombatInfo.iHp < (int)((float)(m_iMaxHp) * (80.f / 100.f)))
	{
		if (!m_bPage_Start)
		{
			m_bPage_Start = true;
			m_eSelected_Pattern = SERPENT::CIRCLE_BULLET;
		}
		m_eSerpentPagePattern = SERPENT::PAGE2;
	}
	else
		m_eSerpentPagePattern = SERPENT::PAGE1;
}

void CSerpent::Ai_State()
{
	switch (m_eSelected_Pattern)
	{
		case SERPENT::CIRCLE_BULLET:
			if (PatternTime())
			{
				if (m_iCircleIndex < 40)
				{
					float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
					m_fAttackTime += fTime;
					if (m_fAttackTime > m_fAttackSpeed)
					{
						m_iCircleIndex += 3;
						if (!static_cast<CSerpentObject*>(m_vecSerpent[m_iCircleIndex])->Get_Break())
						{
							static_cast<CSerpentObject*>(m_vecSerpent[m_iCircleIndex])->Set_Pattern(SERPENT::CIRCLE_BULLET);
							static_cast<CSerpentObject*>(m_vecSerpent[m_iCircleIndex])->Set_AniStart();
							m_iCircleEndIdx = m_iCircleIndex;
						}
						m_fAttackTime = 0.f;
					}
				}
				else
				{
					if (static_cast<CSerpentObject*>(m_vecSerpent[m_iCircleEndIdx])->Get_Attack_End())
					{
						m_eSelected_Pattern = SERPENT::END;
						m_iCircleIndex = 0;
						for (m_iCircleIndex = 0; m_iCircleIndex < 40; m_iCircleIndex += 3)
						{
							if (m_iCircleIndex < 40)
							{
								m_iCircleIndex += 3;
								static_cast<CSerpentObject*>(m_vecSerpent[m_iCircleIndex])->Set_Attack_End(false);
								m_eSelected_Pattern = SERPENT::END;
								m_bPattern_Start = false;

							}
						}
					}
				}
			}
		break;
		case SERPENT::RAGER:
			(m_pChargeBeam)->Set_Pos(m_tInfo.vPos+m_tInfo.vDir*3.f);
			if (static_cast<CSerpentObject*>(m_vecSerpent.front())->Get_Attack_End())
			{
				static_cast<CEffect*>(m_pChargeBeam)->Set_FrameStart(false);
				static_cast<CEffect*>(m_pChargeBeam)->Set_Size(0.f);
				static_cast<CEffect*>(m_pChargeBeam)->Set_ReduceTime(0.f);
				for (auto& iter : m_vecSerpent)
					iter->Set_TrueMod(false);
				m_eSelected_Pattern = SERPENT::END;
				m_bPattern_Start = false; 
				static_cast<CSerpentObject*>(m_vecSerpent.front())->Set_Attack_End(false);
			}
			break;
		case SERPENT::END:
			if (PatternTime())
			{
				m_bPattern_Start = false;
				if (!m_bCircle_Rager)
					m_eSelected_Pattern = SERPENT::RAGER;
				else
					m_eSelected_Pattern = SERPENT::CIRCLE_BULLET;
				m_bCircle_Rager = !m_bCircle_Rager;
			}
			break;
	}

	Move();
}

void CSerpent::Move()
{
	m_vTarget_Dir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
	if (m_eSelected_Pattern == SERPENT::CIRCLE_BULLET)
	{
		m_vTarget_Dir = m_vEnd_Dir -m_tInfo.vPos;
	}
	TargetAngle_Check();
	if (!m_bGravity )
		Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	else
		Accel(m_vGravity, m_fGravity, m_fMaxSpeed, true);
}





void CSerpent::Init_Target()
{
	//Target ����
	m_pTarget = CGameObject_Manager::Get_Instance()->Get_Player();
	for (int i = 0; i < m_iMaxBody; i++)
	{
		if (i == 0)
		{
			m_vecSerpent[0]->Set_Target(this);
			continue;
		}
		m_vecSerpent[i]->Set_Target(m_vecSerpent[i - 1]);
	}
}

void CSerpent::Init_Pos()
{
	//pos ����
	for (int i = 0; i <m_iMaxBody; i++)
	{
		if (i == 0)
		{
			m_vecSerpent[0]->Set_Pos(m_tInfo.vPos);
			continue;
		}
		m_vecSerpent[i]->Set_Pos(_vec3{ m_vecSerpent[i - 1]->Get_ObjInfo().vPos.x - (m_vecSerpent[i - 1]->Get_Texture()->tImageInfo.Width),m_tInfo.vPos.y,0.f });
	}
}

void CSerpent::Init_Hp()
{
	//hp ���� �� ������Ʈ �߰� 
	auto iter = m_vecSerpent.rbegin();
	for (; iter != m_vecSerpent.rend(); iter++)
	{
		m_iMaxHp += static_cast<CSerpentObject*>(*iter)->Get_CombatInfo().iHp;
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::ENEMY), *iter);
	}

	m_tCombatInfo.iHp = m_iMaxHp;
}


bool CSerpent::PatternTime()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fPatternTime += fTime;
	if (m_fPatternTime > m_fPatternEnd)
	{
		return true;
	}
	return false;
}

bool CSerpent::Hp_RedTime()
{
	float fTime = CTime_Manager::Get_Instance()->Get_DeltaTime();
	m_fHp_RedTime += fTime;

	if(m_fHp_RedTime>m_fHp_RedSpeed)
	{
		return true;
	}
	return false;
}
