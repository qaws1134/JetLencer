#include "stdafx.h"
#include "DmgGrid.h"
#include "Prefab_Manager.h"
#include "Gui.h"
#include "GameObject_Manager.h"
CDmgGrid::CDmgGrid()
{
	m_vecGrid.reserve(TILEX*TILEY);
}


CDmgGrid::~CDmgGrid()
{
}

CGameObject * CDmgGrid::Create()
{
	CGameObject* pInstance = new CDmgGrid;
	const OBJECTINFO* pObjInfo = CPrefab_Manager::Get_Instance()->Get_ObjectPrefab(L"Particle");
	pInstance->Set_Prefab(pObjInfo);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		delete pInstance;
		pInstance = nullptr;
		return pInstance;
	}
	return pInstance;

}

HRESULT CDmgGrid::Ready_GameObject()
{
	m_tFrame.wstrObjKey = m_pObjectInfo->wstrIdleImage_ObjectKey;
	m_tFrame.wstrStateKey = m_pObjectInfo->wstrIdleImage_StateKey;
	m_tFrame.fMaxFrame = 1;
	m_tFrame.fStartFrame = 0;

	m_tInfo.vPos = { float(WINCX >> 1),float(WINCY >> 1),0.f};
	m_eRenderId = RENDERID::UI;
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), this);
	m_iRenderIdx = 5;

	float fX = 0.f;
	float fY = 0.f;
	CGameObject* pGrid = nullptr;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			fX = float((j * TILECX) + ((i % 2) * (TILECX>>1)));
			fY = float(i * (TILECY *0.5f));
			pGrid = CGui::Create(UI::DMGGRID);
			pGrid->Set_Pos(_vec3{ fX,fY,0.f });
			pGrid->Set_Size(_vec3{ 1.5f, 1.5f, 1.f });
			m_vecGrid.emplace_back(pGrid);
		}
	}

	for (auto& iter : m_vecGrid)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager((OBJID::UI), iter);
	}

	return S_OK;
}

int CDmgGrid::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CDmgGrid::State_Change()
{
	if (m_bUiGreen)
	{
		for (auto& iter : m_vecGrid)
		{
			static_cast<CUi*>(iter)->Set_Render(true);
			iter->Set_Color(MATCOLOR{ 255,255,255,255 });
			static_cast<CGui*>(iter)->Set_Green(true);

			iter->Set_Alpha(rand() % 180+20);
			if (rand() % 4 == 0)
				static_cast<CUi*>(iter)->Select_Frame(1.f);
			else
				static_cast<CUi*>(iter)->Select_Frame(0.f);

			static_cast<CGui*>(iter)->Set_Action(true);
			static_cast<CGui*>(iter)->Set_Timer(((float)(rand() % 20 + 20)*0.05f));
		}
		Remove_Grid();
		m_bUiGreen = false;
	}
	else if (m_bUiRed)
	{
		//들어왔을때 값 초기화
		for (auto& iter : m_vecGrid)
		{
			static_cast<CUi*>(iter)->Set_Render(true);
			iter->Set_Color(MATCOLOR{ 255,255,255,255 });
			static_cast<CGui*>(iter)->Set_Red(true);

			iter->Set_Alpha(rand() % 180+20);
			if(rand() % 4 == 0 )
				static_cast<CUi*>(iter)->Select_Frame(1.f);
			else
				static_cast<CUi*>(iter)->Select_Frame(0.f);

			static_cast<CGui*>(iter)->Set_Action(true);
			static_cast<CGui*>(iter)->Set_Timer(((float)(rand()%20+20)*0.05f)+1.f);
		}
		Remove_Grid();
		m_bUiRed = false;
	}
}

void CDmgGrid::Remove_Grid()
{
	for (int i = 0; i < TILEY; i++)
	{
		if (i == 0)
			continue;
		for (int j = 0; j < TILEX; j++)
		{
			int iIdx = i * TILEX + j;
			if (j > m_iRenderIdx&&j < TILEX - m_iRenderIdx)
			{
				static_cast<CUi*>(m_vecGrid[iIdx])->Set_Render(false);
			}
		}
		if (i >(TILEY >> 1)+1)
		{
			if (m_iRenderIdx <5)
				m_iRenderIdx++;
		}
		else
		{
			if (m_iRenderIdx > 3)
				m_iRenderIdx--;
		}
	}
}

