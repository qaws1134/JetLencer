#include "stdafx.h"
#include "Jet_Normal.h"
#include "Spawn_Manager.h"
#include "ColSphere.h"
CJet_Normal::CJet_Normal()
{
}


CJet_Normal::~CJet_Normal()
{
}


CGameObject * CJet_Normal::Create(const OBJECTINFO * _tObjectInfo, _vec3 _vPos)
{
	CGameObject* pInstance = new CJet_Normal;
	pInstance->Set_Prefab(_tObjectInfo);
	pInstance->Set_Pos(_vPos);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;
}

HRESULT CJet_Normal::Ready_GameObject()
{
	InitJet();

	m_tCombatInfo.iAtk = 1;
	m_tCombatInfo.iHp = 5;

	m_fAttackTime = 5.f;
	m_fAttackSpeed = 0.08f;

	m_fAccel = 1800.f;
	m_fMaxSpeed = 500.f;

	m_eJetState = ENEMY::ATTACK;
	m_fAngle_per_Frame = 45.f;
	m_fAngleOffset = -22.5f;
	m_vecCollider.reserve(1);
	m_vecCollider.emplace_back(CColSphere::Create(this, m_tCombatInfo, 15.f, COLLIDER::ENEMY));

	return S_OK;
}

void CJet_Normal::State_Change()
{
	//Ÿ�� �ޱ��� ���� �� offset�� ���ؼ� �ޱ��� �ش� 
	//offset���� ���� 
	if (m_ePreJetState!= m_eJetState)
	{
		switch (m_eJetState)
		{
		case ENEMY::ATTACK:
			//m_fAngleOffset = float(rand() % 10);			
			
			//offsetAngle ���ϱ�
			//Ÿ���� �÷��̾�
			//Ÿ���� �ڽ��� dir���⿡�� ���� ���� ���� ���� ������ �Ѿ� �߻� ������ +-5������??



			break;

		case ENEMY::BACK:
			//Ÿ���� ȭ�� �� �ƹ����� 
			//�ö��̴��� ���� �ö��̴� �ȿ� ������ ����, ���� ������ �ö��̴��� �����
			//Ȥ�� �ö��̴� ���ο��� ���� �ð��� ������ �ٽ� ���� ���·� ����
			



			break;
		}
		m_ePreJetState = m_eJetState;
	}
}

void CJet_Normal::Ai_State()
{
	Accel(m_tInfo.vDir, m_fAccel, m_fMaxSpeed, false);
	m_bAccel = true;
	SpawnPtFire();
	if (m_eJetState == ENEMY::ATTACK)
	{
		float fDis = D3DXVec3Length(&(m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos));
		float fRad = 400.f;
		m_vTarget_Dir = m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos;
		TargetAngle_Check();
		if ((fDis <= fRad)&& AttackTime())
		{
			CSpawn_Manager::Spawn(L"EnemyBullet", m_tInfo.vPos, m_fAngle, m_vVelocity);
			m_eJetState = ENEMY::BACK;
		}
	}
	else
	{
		float fDis = D3DXVec3Length(&(m_pTarget->Get_ObjInfo().vPos - m_tInfo.vPos));
		float fRad = 600.f;
		m_vTarget_Dir = m_tInfo.vPos-m_pTarget->Get_ObjInfo().vPos;
		TargetAngle_Check();

		if (fDis >= fRad)
		{
			m_eJetState = ENEMY::ATTACK;
		}
	}

	//����
	//�����ϰ��� state ���� 
	//Ÿ���� �÷��̾�
	//Ÿ���� �ڽ��� dir���⿡�� ���� ���� ���� ���� ������ �Ѿ� �߻� ������ +-5������??
	
	//����
	//�÷��̾��� ���� �ö��̴��� ���� �ö��̴� ���� ȸ�ǻ��� ���� ���ݻ��� 
	//���� ������ �ö��̴��� ����� -> auto k??
	//
	//Ȥ�� �ö��̴� ���ο��� ���� �ð��� ������ �ٽ� ���� ���·� ����
}

void CJet_Normal::DeadEffect()
{
	CSpawn_Manager::Spawn(L"Dead_Explosion1", m_tInfo.vPos);
	m_pBurner->Set_Dead(true);
	m_pBurner = nullptr;
	m_bDead = true;
}

