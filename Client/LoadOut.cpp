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
	//�����Ӱ� ����� ���Ϳ� insert
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


	//�������� ���Ϳ� insert

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



	////���� ��
	pObject = CGui::Create(UI::BUTTON);
	static_cast<CGui*>(pObject)->Select_Frame(1.f);
	pObject->Set_Pos(_vec3{20.f,50.f,0.f});
	m_vecUiFrame.emplace_back(pObject);
	pObject = CViewText::Create(L"LOADOUT",TEXT::BIG);
	pObject->Set_Pos(_vec3{ 20.f,22.f,0.f });
	m_vecUiFrame.emplace_back(pObject);


	//������ ��
	pObject = CGui::Create(UI::BUTTON);
	pObject->Set_Color(MATCOLOR{ 180,255,255,255 });
	pObject->Set_Size(_vec3{ 0.8f, 0.8f, 0.f });
	static_cast<CGui*>(pObject)->Select_Frame(1.f);
	pObject->Set_Pos(_vec3{ float(WINCX)-300.f,50.f,0.f });
	m_vecUiFrame.emplace_back(pObject);


	//����â 
	pObject = CGui::Create(UI::SCREENBLACK);
	pObject->Set_Color(MATCOLOR{ 200,255,255,255 });
	pObject->Set_Size(_vec3{1.8f, 1.5f, 0.f });
	pObject->Set_Pos(_vec3{ float(WINCX)-300.f,250.f,0.f });
	m_vecUiFrame.emplace_back(pObject);

	////���� ��� �̸� 
	pObject = CViewText::Create(L"::���� �̻���::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 350.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::�� ���� �̻���::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 360.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::�뵹�� ����::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 350.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"::������ ���� �̻���::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 400.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::PBG (Power BeamLaser is Good)::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 480.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::�ڵ� �Ѿ� �߻��::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 380.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"::���� �ڵ� ���̵�::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 380.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::�ν��ͷ� ����::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 360.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"::���� �ӵ� ����::");
	pObject->Set_Pos(_vec3{ float(WINCX) - 370.f,33.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);


	//����, ��� ����
	pObject = CViewText::Create(L"OBJ �Ŵ��� ��Ÿ�� ����ID �Ѱ��ָ�\n���� �����ؼ� ���ϳ��󰩴ϴ�.\n�ٷ� �� ����!!\n\n\n\n\n#TMI ",TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"�����... ����ź �� ���� �־��µ� �ѹ��� ���ش�\n\n��� �� �� ���� ���� ��ġ�� ����", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"�뵹�� ��ź�� 3�� �����ϴ�.\n�� ���� �����İ� �Ǿ� �Ͻð����� \n�� ������ ������ ���� �����ϴ�....\n�������� 3.��.��.��\n�� ��.... �� ��������?...\n\n\n\n#�뵹�� #�� �� #������ �߷��� �ִ� \n#���� ������", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"���� �̻����� �ѹ��� ������ �̴´ٱ�?...\n\n\n#����� #���� ã�ư��� ������....", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"PBG�� ���� �߻��Ѵ�\n.\n\n\n\n\n\n\n\n#���� ���� #like PBG #������� ", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"�� ����� ����� �ֺ� ������ ���� ���ϴ�\n", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);

	pObject = CViewText::Create(L"�ٴڿ� �������� �; ������ �� �����ϴ�.\n�ڵ����� �ٽ� ���� �÷��ִ� ��÷�� ���\n\n���� �ӵ���+��ġ�� �ٴ��̸� \n���� �ڵ����� �������ݴϴ�\n�ٴڿ� �ε����� �����ϱ��...\n\n\n#�ٴ� �ǻ� ���� #�ٴ��ϰ� #���� ����", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"�ν��� �ð��� �÷��ݴϴ�", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	pObject = CViewText::Create(L"������ �ð��� �� �� ��������ݴϴ�", TEXT::SMALL);
	pObject->Set_Pos(_vec3{ float(WINCX) - 500.f,120.f,0.f });
	m_vecLoadOutInfo.emplace_back(pObject);
	//��� ��ġ�� ��ũ�� �� ��ġ

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


	// ��ġ�������ֱ�
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
	//����
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
