#include "stdafx.h"
#include "LoadOut.h"
#include "Gui.h"
#include "ViewText.h"
#include "Scene_Manager.h"
#include "SoundMgr.h"
CLoadOut::CLoadOut():m_bChangeIndex(true), m_iTextLineInc(0), m_bDead(false)
{
	m_vecLoadOutFrame.reserve(16);
	m_vecLoadOutIcon.reserve(16);
	m_vecLoadOutBackplate.reserve(16);
	m_vecLoadOutInfo.reserve(32);
	m_vecUiFrame.reserve(16);
	m_vecLoadOutEquip.reserve(8);
}


CLoadOut::~CLoadOut()
{
	Release_Scene();
}

CScene * CLoadOut::Create()
{
	CScene* pInstance = new CLoadOut;
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	return pInstance;
}

HRESULT CLoadOut::Ready_Scene()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"LoadOut.mp3");
	m_iMaxWeaponFrame = 9;
	m_iPointIndex = 0;
	m_pBackGround = CGui::Create(UI::LO_BACKGROUND);
	m_pBackGround->Set_Pos(_vec3{ float(WINCX >> 1),float(WINCY >> 1),0.f });
	m_pBackGround->Set_Size(_vec3{ 1.3f,1.3f,0.f });

	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pBackGround);

	CGameObject* pObject = nullptr;
	//프레임과 배경을 벡터에 insert
	for (int i = 0; i < m_iMaxWeaponFrame; i++)
	{
		pObject = CGui::Create(UI::LO_FRAME);
		pObject->Set_Size(_vec3{ 2.f,2.f,0.f });
		m_vecLoadOutFrame.emplace_back(pObject);

		pObject = CGui::Create(UI::LO_BACKPLATE);
		static_cast<CUi*>(pObject)->Set_Render(false);
		static_cast<CGui*>(pObject)->Select_Frame(1.f);
		pObject->Set_Size(_vec3{ 2.f,2.f,0.f });
		m_vecLoadOutBackplate.emplace_back(pObject);
	}


	//아이콘을 벡터에 insert

	for (int i = 0; i < 3; i++)
	{
		pObject = CGui::Create(UI::LO_SUBWEAPON);
		static_cast<CGui*>(pObject)->Select_Frame(float(i));
		pObject->Set_Size(_vec3{ 2.f,2.f,0.f });
		m_vecLoadOutIcon.emplace_back(pObject);
	}
	for (int i = 0; i < 3; i++)
	{
		pObject = CGui::Create(UI::LO_CHARGEWEAPONE);
		static_cast<CGui*>(pObject)->Select_Frame(float(i));
		pObject->Set_Size(_vec3{ 2.f,2.f,0.f });
		m_vecLoadOutIcon.emplace_back(pObject);
	}
	for (int i = 0; i < 3; i++)
	{
		pObject = CGui::Create(UI::LO_MODULE);
		static_cast<CGui*>(pObject)->Select_Frame(float(i));
		pObject->Set_Size(_vec3{ 2.f,2.f,0.f });
		m_vecLoadOutIcon.emplace_back(pObject);
	}

	m_pPointer = CGui::Create(UI::LO_POINTER);
	m_pPointer->Set_Size(_vec3{ 2.f,2.f,0.f });


	for (int i = 0; i < 3; i++)
	{
		pObject = CGui::Create(UI::LO_TEXTLINEYELLO);
		pObject->Set_Pos(_vec3{ 310.f,float(i * 220) + 120,0.f });
		pObject->Set_Size(_vec3{ 2.5f,2.f,0.f });
		if(i ==2 )
			static_cast<CGui*>(pObject)->Select_Frame(3.f);
		else
			static_cast<CGui*>(pObject)->Select_Frame(1.f);
		m_vecUiFrame.emplace_back(pObject);

		pObject = CGui::Create(UI::LO_TEXTLINERED);
		pObject->Set_Pos(_vec3{ 300.f,float(i * 220) + 120,0.f });
		pObject->Set_Size(_vec3{ 2.5f,2.f,0.f });
		static_cast<CGui*>(pObject)->Select_Frame(0.f);
		m_vecUiFrame.emplace_back(pObject);
	
	}


	pObject = CViewText::Create(L"::SPECIAL WEAPON::");
	pObject->Set_Pos(_vec3{ 20.f, 100.f ,0.f});
	m_vecUiFrame.emplace_back(pObject);

	pObject = CViewText::Create(L"::CHARGE WEAPON::");
	pObject->Set_Pos(_vec3{ 20.f, 320.f ,0.f});
	m_vecUiFrame.emplace_back(pObject);

	pObject = CViewText::Create(L"::INSTALLED MODULE::");
	pObject->Set_Pos(_vec3{ 20.f,float(2 * 270),0.f });
	m_vecUiFrame.emplace_back(pObject);



	////왼쪽 위
	pObject = CGui::Create(UI::BUTTON);
	static_cast<CGui*>(pObject)->Select_Frame(1.f);
	pObject->Set_Pos(_vec3{20.f,50.f,0.f});
	m_vecUiFrame.emplace_back(pObject);
	pObject = CViewText::Create(L"LOADOUT",TEXT::BIG);
	pObject->Set_Pos(_vec3{ 20.f,22.f,0.f });
	m_vecUiFrame.emplace_back(pObject);


	//오른쪽 위
	pObject = CGui::Create(UI::BUTTON);
	pObject->Set_Color(MATCOLOR{ 180,255,255,255 });
	pObject->Set_Size(_vec3{ 0.8f, 0.8f, 0.f });
	static_cast<CGui*>(pObject)->Select_Frame(1.f);
	pObject->Set_Pos(_vec3{ float(WINCX)-300.f,50.f,0.f });
	m_vecUiFrame.emplace_back(pObject);


	//설명창 
	pObject = CGui::Create(UI::SCREENBLACK);
	pObject->Set_Color(MATCOLOR{ 200,255,255,255 });
	pObject->Set_Size(_vec3{1.8f, 1.5f, 0.f });
	pObject->Set_Pos(_vec3{ float(WINCX)-300.f,250.f,0.f });
	m_vecUiFrame.emplace_back(pObject);

	////웨펀 모듈 이름 
	pObject = CViewText::Create(L"::유도 미사일::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 350.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::안 유도 미사일::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 360.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::통돌이 샷건::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 350.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"::때거지 유도 미사일::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 400.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::PBG (Power BeamLaser is Good)::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 480.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::자동 총알 발사기::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 380.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"::지면 자동 가이드::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 380.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::부스터량 증가::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 360.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::차지 속도 감소::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 370.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);


	//웨펀, 모듈 설명
	pObject = CViewText::Create(L"OBJ 매니져 겟타겟 몬스터ID 넘겨주면\n몬스터 추적해서 로켓날라갑니다.\n바로 그 로켓!!\n\n\n\n\n#TMI ",TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"적들아... 유도탄 쏠 수도 있었는데 한번만 봐준다\n\n대신 쫌 더 강한 매콤 펀치가 간다", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"통돌이 폭탄을 3개 날립니다.\n왜 통을 날리냐고 의아 하시겠지만 \n이 게임은 실제로 통을 날립니다....\n샷건으로 3.발.이.나\n이 통.... 좀 강할지도?...\n\n\n\n#통돌이 #좀 쌤 #지구는 중력이 있다 \n#점점 떨어져", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"유도 미사일을 한번에 여러발 뽑는다구?...\n\n\n#적들아 #굳이 찾아가진 않을게....", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"PBG의 빛을 발사한다\n.\n\n\n\n\n\n\n\n#멋짐 보장 #like PBG #비쥬얼담당 ", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"이 드론은 무료로 주변 적에게 총을 쏩니다\n", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"바닥에 떨어지고 싶어도 떨어질 수 없습니다.\n자동으로 다시 위로 올려주는 최첨단 기술\n\n다음 속도값+위치가 바닥이면 \n위로 자동으로 움직여줍니다\n바닥에 부딪히면 아프니까요...\n\n\n#바닥 뽀뽀 금지 #바닥하고도 #연애 금지", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"부스터 시간을 늘려줍니다", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"차지샷 시간을 좀 더 단축시켜줍니다", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	//모두 위치는 스크린 블랙 위치

	_vec3 vFrameOffset = { 80.f,210.f,0.f };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int iIdx = (i * 3) + j;
			m_vecLoadOutFrame[iIdx]->Set_Pos(_vec3{float((j)*180)+ vFrameOffset.x,float(((i))*220)+ vFrameOffset.y,0.f});
			m_vecLoadOutBackplate[iIdx]->Set_Pos(_vec3{ float((j) * 180) + vFrameOffset.x,float(((i)) * 220) + vFrameOffset.y,0.f });
			m_vecLoadOutIcon[iIdx]->Set_Pos(_vec3{ float((j) * 180) + vFrameOffset.x,float(((i)) * 220) + vFrameOffset.y,0.f });
			m_vecFrameIndex.emplace_back(_vec3{ float((j) * 180) + vFrameOffset.x,float(((i)) * 220) + vFrameOffset.y,0.f });
		}
	}



	for (auto& iter : m_vecUiFrame)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}

	for (auto& iter : m_vecLoadOutBackplate)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}
	for (auto& iter : m_vecLoadOutIcon)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);
	}
	for (auto& iter : m_vecLoadOutFrame)
	{
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI,iter);
	}

	for (auto& iter : m_vecLoadOutInfo)
	{
		static_cast<CUi*>(iter)->Set_Render(false);
		CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, iter);

	}


	// 위치값정해주기
	m_pPointer->Set_Pos(m_vecFrameIndex[0]);
	CGameObject_Manager::Get_Instance()->Add_GameObject_Manager(OBJID::UI, m_pPointer);

	list<LOADOUT::INDEX> _eLoadOutSavedData  = CGameObject_Manager::Get_Instance()->Get_LoadOutData();

	for (auto& iter : _eLoadOutSavedData) 
	{
		Select_LoadOut(int(iter));
		static_cast<CUi*>(m_vecLoadOutBackplate[int(iter)])->Set_Render(true);
	}

	return S_OK;
}

void CLoadOut::Update_Scene()
{
	if (m_bDead)
		return;
	m_pPointer->Set_Pos(m_vecFrameIndex[m_iPointIndex]);
	Show_Info();
	Key_Check();
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CLoadOut::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CLoadOut::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);

}

void CLoadOut::Key_Check()
{
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_W))
	{
		if (m_iPointIndex >= 3)
		{
			m_iPointIndex -= 3;
			m_bChangeIndex = true;
			CSoundMgr::Get_Instance()->StopSound( CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_S))
	{
		if (m_iPointIndex <6)
		{
			m_iPointIndex += 3;			
			m_bChangeIndex = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_A))
	{
		if (m_iPointIndex % 3 != 0)
		{
			m_iPointIndex--;
			m_bChangeIndex = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_D))
	{
		if (m_iPointIndex % 3 != 2)
		{
			m_iPointIndex++;
			m_bChangeIndex = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_MOVE);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Move.mp3", CSoundMgr::UI_MOVE);
		}
	}
	//착용
	if (CKey_Manager::Get_Instance()->Key_Down(KEY_SPACE))
	{
		Select_LoadOut(m_iPointIndex);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_SELECT);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Select.mp3", CSoundMgr::UI_SELECT);
	}


	if (CKey_Manager::Get_Instance()->Key_Down(KEY_ESC))
	{
		CGameObject_Manager::Get_Instance()->Clear_LoadOutList();
		for(size_t i = 0;i <m_vecLoadOutBackplate.size();i++)
		{
			if (static_cast<CGui*>(m_vecLoadOutBackplate[i])->Get_Start())
			{
				CGameObject_Manager::Get_Instance()->Set_LoadOutData(LOADOUT::INDEX(i));
			}
		}
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_SELECT);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Select.mp3", CSoundMgr::UI_SELECT);
		CGameObject_Manager::Get_Instance()->DeleteID_GameObject_Manager(OBJID::UI);
		CScene_Manager::Get_Instance()->Change_Scene_Manager(CScene_Manager::SCENE_MENU);
		m_bDead = true;
	}
}




void CLoadOut::Show_Info()
{
	if (m_bChangeIndex)
	{
		for (auto& iter : m_vecLoadOutInfo)
			static_cast<CUi*>(iter)->Set_Render(false);
		for (auto& iter : m_vecLoadOutBackplate)
		{
			if (static_cast<CGui*>(iter)->Get_Start())
			{
				static_cast<CGui*>(iter)->Select_Frame(0.f);
				continue;
			}
			static_cast<CUi*>(iter)->Set_Render(false);
		}
		static_cast<CUi*>(m_vecLoadOutInfo[m_iPointIndex])->Set_Render(true);
		static_cast<CUi*>(m_vecLoadOutInfo[(m_vecLoadOutInfo.size() / 2) + m_iPointIndex])->Set_Render(true);
		static_cast<CGui*>(m_vecLoadOutBackplate[m_iPointIndex])->Select_Frame(1.f);
		static_cast<CUi*>(m_vecLoadOutBackplate[m_iPointIndex])->Set_Render(true);


		m_bChangeIndex = false;
	}

}

void CLoadOut::Select_LoadOut(int _iIdx)
{
	if ((_iIdx / 3 < 1) || (_iIdx / 3) < 2)
	{
		for (int i = 0; i < 3; i++)
		{
			static_cast<CGui*>(m_vecLoadOutBackplate[(_iIdx / 3)*3 + i])->Set_Start(false);
		}
		static_cast<CGui*>(m_vecUiFrame[(_iIdx / 3)*2+1])->Select_Frame(1.f);
		static_cast<CGui*>(m_vecLoadOutBackplate[_iIdx])->Set_Start(true);
	}
	else
	{
		if (static_cast<CGui*>(m_vecLoadOutBackplate[_iIdx])->Get_Start())
		{
			static_cast<CGui*>(m_vecLoadOutBackplate[_iIdx])->Set_Start(false);
			m_iTextLineInc--;
		}
		else
		{
			static_cast<CGui*>(m_vecLoadOutBackplate[_iIdx])->Set_Start(true);
			m_iTextLineInc++;
		}
		static_cast<CGui*>(m_vecUiFrame[(_iIdx / 3) * 2 + 1])->Select_Frame(float(m_iTextLineInc));
	}
	m_bChangeIndex = true;
}
