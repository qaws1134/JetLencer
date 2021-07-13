// AnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"


// CAnimationTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
	, m_wstrObject_Key(_T("")), m_wstrState_Key(_T(""))
	,m_fPlay_Speed(0.f),m_iMax_Index(0)
	, m_iInput_Index(0)
	, m_iDrawID(0), m_iAnimationCount(0)
	, m_iPlay_Speed(0)
{

}

CAnimationTool::~CAnimationTool()
{

	for (pair<CString, ANIMATION*> rPair : m_mapAnima)
		Safe_Delete(rPair.second);
	m_mapAnima.clear();

}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_wstrObject_Key);
	DDX_Text(pDX, IDC_EDIT2, m_wstrState_Key);
	DDX_Text(pDX, IDC_EDIT3, m_fPlay_Speed);
	DDX_Text(pDX, IDC_EDIT4, m_iMax_Index);
	DDX_Text(pDX, IDC_EDIT5, m_iInput_Index);

	DDX_Control(pDX, IDC_START_IMAGE, m_Start_Image);
	DDX_Control(pDX, IDC_END_IMAGE, m_End_Image);
	DDX_Control(pDX, IDC_INDEX_TO_SELECTED_IMAGE, m_Index_To_Selected_Image);
	DDX_Control(pDX, IDC_LIST_TO_SELECTED_IMAGE, m_List_To_Selected_Image);

	DDX_Control(pDX, IDC_LIST2, m_Image_ListBox);
	DDX_Control(pDX, IDC_LIST1, m_Animation_ListBox);

	DDX_Control(pDX, IDC_CHECK2, m_IsSingle);
	DDX_Control(pDX, IDC_CHECK1, m_Loop);
	DDX_Text(pDX, IDC_EDIT6, m_iPlay_Speed);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimationTool::OnBnClickedIndex_Apply)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationTool::OnBnClickedAddAnimation)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimationTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationTool::OnLbnSelchangeImageList)

	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationTool::OnLbnSelchangeAnimation_List)
	ON_WM_DROPFILES()

	ON_BN_CLICKED(IDC_BUTTON10_ANIM, &CAnimationTool::OnBnClickedAddInfo)
	ON_BN_CLICKED(IDC_BUTTON11, &CAnimationTool::OnBnClickedDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON12, &CAnimationTool::OnBnClickedDeleteAnimationList)
	ON_BN_CLICKED(IDC_BUTTON13, &CAnimationTool::OnBnClickedClearBox)
	ON_BN_CLICKED(IDC_BUTTON14, &CAnimationTool::OnBnClickedClear_Info)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON16, &CAnimationTool::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON15, &CAnimationTool::OnBnClickedStop)
END_MESSAGE_MAP()


// addinfo 버튼 클릭
void CAnimationTool::OnBnClickedAddInfo()
{
	UpdateData(TRUE);
	//==========animation 정보 입력=======================
	ANIMATION* pAnima = new ANIMATION{};
	CString AnimationName = m_wstrObject_Key + m_wstrState_Key; //두개objkey와 statekey를 더한것이 키값 

	pAnima->wstrObjectKey = m_wstrObject_Key;
	pAnima->wstrStateKey = m_wstrState_Key;
	pAnima->fPlay_Speed = m_fPlay_Speed;
	pAnima->iMax_Index = m_iMax_Index;

	if (m_Loop.GetCheck())
		pAnima->bLoop = true;
	else
		pAnima->bLoop = false;

	if (m_IsSingle.GetCheck())
		pAnima->bIsSingle = true;
	else
		pAnima->bIsSingle = false;
	//=====================================================

	m_mapAnima.emplace(AnimationName, pAnima);
	ERR_MSG(L"정보 입력 성공");
	UpdateData(FALSE);
}


void CAnimationTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	int iShotGunCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	m_Drop = hDropInfo;


	for (int i = 0; i < iShotGunCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);

		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);

		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		m_Image_ListBox.AddString(szFileName);
	}
	m_Image_ListBox.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);



	CString wstrSingleFilePath = CFileInfo::ConvertRelativePath(szFileFullPath);
	
	//===============멀티 텍스쳐 파일 이름 셋팅====================
	CString wstrFileNameAndEx = PathFindFileName(wstrSingleFilePath);
	CString wstrMultiFilePath = wstrSingleFilePath;
	
	TCHAR szFileName[MAX_PATH]{};
	lstrcpy(szFileName, wstrFileNameAndEx.GetString());
	int iPathLength = wstrMultiFilePath.GetLength() - lstrlen(szFileName);
	wstrMultiFilePath.Delete(iPathLength, lstrlen(szFileName));
	PathRemoveExtension(szFileName);

	int i = lstrlen(szFileName)-1;
	for (; i>=0; i--)
	{
		if (!isdigit(szFileName[i]))
		{
			break;
		}
		szFileName[i] = NULL;
	}
	lstrcat(szFileName, L"%d.png");

	wstrMultiFilePath.Insert(iPathLength, szFileName);
	
	//===========================================
	//이미지 insert
	if (m_IsSingle.GetCheck())
	{
		if (m_wstrObject_Key != L"")
		{
			m_wstrFilePath = wstrSingleFilePath;
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrSingleFilePath.GetString(), m_wstrObject_Key.GetString())))
			{
				ERR_MSG(L"싱글 텍스쳐 실패");
				return;
			}
		}
	}
	else
	{
		if (m_wstrObject_Key != L""&& m_wstrState_Key != L"")
		{
			m_wstrFilePath = wstrMultiFilePath;
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, wstrMultiFilePath.GetString(), m_wstrObject_Key.GetString(), m_wstrState_Key.GetString(), iShotGunCount)))
			{
				ERR_MSG(L"멀티 텍스쳐 실패");
				return;
			}
		}

	}
}


//끌어넣은 이미지 리스트 선택 후 확인
void CAnimationTool::OnLbnSelchangeImageList()
{
	int iListCursor = m_Image_ListBox.GetCurSel();
	CString wstrFileName;
	m_Image_ListBox.GetText(iListCursor, wstrFileName);

	int i = wstrFileName.GetLength()-1;
	for (; i >= 0; --i)
	{
		if (!isdigit(wstrFileName[i]))
			break;
	}
	wstrFileName.Delete(0, i+1);
	m_iDrawID = _ttoi(wstrFileName.GetString());
	SetImageView(m_wstrObject_Key.GetString(), m_iDrawID, m_List_To_Selected_Image, m_wstrState_Key.GetString());
}


//스테틱 박스에 이미지 출력을 위한 함수
void CAnimationTool::SetImageView(CString Objectkey, int Index,const CStatic& PictureBox, CString StateKey)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(Objectkey.GetString(), StateKey.GetString(), Index);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Render_End(PictureBox.m_hWnd);
}

//인포를 리셋시킴
void CAnimationTool::Reset_Info()
{

	m_wstrObject_Key = L"";
	m_wstrState_Key = L"";
	m_fPlay_Speed = 0.f;
	m_iMax_Index = 0;
	m_IsSingle.SetCheck(FALSE);
	m_Loop.SetCheck(FALSE);

}

//이미지 리스트박스를 리셋시킴
void CAnimationTool::Reset_ImageList()
{
	if (m_Image_ListBox.GetCount() != 0)
		m_Image_ListBox.ResetContent();
}

//애니메이션 최종으로 합쳐서 추가
//파일 정보 map에 저장 후 listbox에 추가 
void CAnimationTool::OnBnClickedAddAnimation()
{
	UpdateData(TRUE);

	auto& iter_find = m_mapAnima.find(m_wstrObject_Key + m_wstrState_Key);
	//맵에 키가 없을 때 리턴 
	if (iter_find == m_mapAnima.end())
	{
		ERR_MSG(L"맵에 일치하는 키가 없다");
		return;
	}
	//파일 리스트가 저장이 안되어있으면 리턴
	if (m_wstrFilePath.IsEmpty())
	{
		ERR_MSG(L"파일 경로 저장되어 있지 않다");
		return;
	}
	//사진 정보 저장
	iter_find->second->wstrFilePath = m_wstrFilePath;

	//리스트 클리어 후 넣기
	//추가 후 이미지 리스트 박스 삭제 
	for (int i = 0; i <m_Image_ListBox.GetCount();i++)
		m_Image_ListBox.ResetContent();


	m_Animation_ListBox.AddString(m_wstrObject_Key+"->"+m_wstrState_Key);

	//start_Image와 End_Index 이미지 출력 
	Reset_Info();


	UpdateData(FALSE);
}

//인덱스에 있는 그림을 확인함
void CAnimationTool::OnBnClickedIndex_Apply()
{
	UpdateData(TRUE);
	int iIndex = m_Animation_ListBox.GetCurSel();

	if (iIndex == LB_ERR)
		return;

	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);

	SetImageView(iter_find->second->wstrObjectKey, m_iInput_Index, m_Index_To_Selected_Image, iter_find->second->wstrStateKey);
	UpdateData(FALSE);

}


void CAnimationTool::OnBnClickedDeleteAll()
{
	UpdateData(TRUE);

	//인포와 이미지 박스 전부 삭제 
	auto& iter_find = m_mapAnima.find(m_wstrObject_Key+m_wstrState_Key);
	CString wstrKey = iter_find->second->wstrObjectKey + "->" + iter_find->second->wstrStateKey;
	for (int i = 0; i < m_Animation_ListBox.GetCount(); i++)
	{
		CString wstrBoxKey;
		m_Animation_ListBox.GetText(i, wstrBoxKey);
		if (wstrKey == wstrBoxKey)
		{
			ERR_MSG(L"이미 만든 이펙트는 여기서 지우면 안된다");
			return;
		}
	}

	if (iter_find == m_mapAnima.end())
	{
		ERR_MSG(L"일치하는 정보가 없다");
		return;
	}
	Reset_ImageList();
	Safe_Delete(iter_find->second);
	m_mapAnima.erase(iter_find);

	Reset_Info();
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		//하나씩 저장 해야지. 폴문 돌면서 . 
		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		// 난 쓸수 있어. 너넨 쓰지마 . 
		for (auto& rPair : m_mapAnima)
		{

			dwStringSize = (rPair.second->wstrObjectKey.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrObjectKey.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrStateKey.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrStateKey.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrFilePath.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrFilePath.GetString(), dwStringSize, &dwbyte, nullptr);


			WriteFile(hFile, &rPair.second->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->iMax_Index, sizeof(int), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->bLoop, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->bIsSingle, sizeof(bool), &dwbyte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CAnimationTool::OnBnClickedLoad()
{
	CFileDialog Dlg(TRUE,// TRUE면 열기. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		for (auto& rPair : m_mapAnima)
			Safe_Delete(rPair.second);
		m_mapAnima.clear();
		m_Animation_ListBox.ResetContent(); 

		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		ANIMATION* pAnimaInfo = 0;
		TCHAR* pBuf = nullptr;

		while (true)
		{
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			if (0 == dwbyte)
				break;
			pBuf = new TCHAR[dwStringSize];
			pAnimaInfo = new ANIMATION;
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrObjectKey = pBuf;
			Safe_Delete(pBuf);
				

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrStateKey = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrFilePath = pBuf;
			Safe_Delete(pBuf);


			ReadFile(hFile, &pAnimaInfo->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->iMax_Index, sizeof(int), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->bIsSingle, sizeof(bool), &dwbyte, nullptr);

			//if (pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey == L"EnemyBullet")
			//{
			//	continue;
			//}
			//if (pAnimaInfo->wstrObjectKey + pAnimaInfo->wstrStateKey == L"EnemyJet_BulletSplash")
			//{
			//	continue;
			//}

			m_mapAnima.emplace(pAnimaInfo->wstrObjectKey+pAnimaInfo->wstrStateKey, pAnimaInfo);
			m_Animation_ListBox.AddString(pAnimaInfo->wstrObjectKey +"->"+pAnimaInfo->wstrStateKey);


			//==============싱글 텍스쳐 파일 이름 셋팅===============
			CString wstrSingleFilePath = pAnimaInfo->wstrFilePath;

			//===============멀티 텍스쳐 파일 이름 셋팅==============
			CString wstrFileNameAndEx = PathFindFileName(wstrSingleFilePath);
			CString wstrMultiFilePath = wstrSingleFilePath;

			TCHAR szFileName[MAX_PATH]{};
			lstrcpy(szFileName, wstrFileNameAndEx.GetString());
			int iPathLength = wstrMultiFilePath.GetLength() - lstrlen(szFileName);
			wstrMultiFilePath.Delete(iPathLength, lstrlen(szFileName));
			PathRemoveExtension(szFileName);

			lstrcat(szFileName, L".png");

			wstrMultiFilePath.Insert(iPathLength, szFileName);


			//===============이미지 Insert==============
			if (pAnimaInfo->bIsSingle)
			{
				if (pAnimaInfo->wstrObjectKey.GetString() != L"")
				{
					if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrSingleFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString())))
					{
						ERR_MSG(L"싱글 텍스쳐 실패");
						return;
					}
				}
			}
			else
			{
				if (pAnimaInfo->wstrObjectKey.GetString() != L""&& pAnimaInfo->wstrStateKey.GetString() != L"")
				{
					if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, wstrMultiFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString(), pAnimaInfo->wstrStateKey.GetString(), pAnimaInfo->iMax_Index)))
					{
						ERR_MSG(L"멀티 텍스쳐 실패");
						return;
					}
				}
			}
		}
		CloseHandle(hFile);
	}
}


//완성된 애니메이션 리스트박스 확인
void CAnimationTool::OnLbnSelchangeAnimation_List()
{
	UpdateData(TRUE);
	Reset_Info();
	Reset_ImageList();

	int iIndex = m_Animation_ListBox.GetCurSel();
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);

	// 키에서 -> 문자 삭제 
	int i = 0;
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}

	wstrFindName.Delete(i-1, 2);
	auto& iter_find = m_mapAnima.find(wstrFindName);

	m_wstrObject_Key = iter_find->second->wstrObjectKey;
	m_wstrState_Key = iter_find->second->wstrStateKey;
	m_fPlay_Speed = iter_find->second->fPlay_Speed;
	m_iMax_Index = iter_find->second->iMax_Index;

	
	if (iter_find->second->bLoop)
		m_Loop.SetCheck(true);
	else
		m_Loop.SetCheck(false);
	if (iter_find->second->bIsSingle)
		m_IsSingle.SetCheck(true);
	else
		m_IsSingle.SetCheck(false);


	//이미지 리스트박스에 저장된 파일 정보 출력 
	int iShotGunCount = iter_find->second->iMax_Index;
	TCHAR szFileFullPath[MAX_PATH]{};

	for (int i = 0; i < iShotGunCount; ++i)
	{
		TCHAR szFilePath[MAX_PATH] = L"";
		swprintf_s(szFilePath, iter_find->second->wstrFilePath, i);
		
		CString wstrFileNameAndEx = PathFindFileName(szFilePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		
		m_Image_ListBox.AddString(szFileName);
	}

	m_Image_ListBox.SetHorizontalExtent(500);

	//start 이미지, end 이미지 출력 
	SetImageView(iter_find->second->wstrObjectKey.GetString(), 0, m_Start_Image, iter_find->second->wstrStateKey.GetString());
	SetImageView(iter_find->second->wstrObjectKey.GetString(), iter_find->second->iMax_Index - 1, m_End_Image, iter_find->second->wstrStateKey.GetString());

	UpdateData(FALSE);
}

void CAnimationTool::OnBnClickedDeleteAnimationList()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_Animation_ListBox.GetCurSel();
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"리스트 먼저 선택해");
		return;
	}
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);

	Safe_Delete(iter_find->second);
	m_mapAnima.erase(iter_find);

	Reset_Info();
	Reset_ImageList();

	m_Animation_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedClearBox()
{
	UpdateData(TRUE);
	Reset_ImageList();
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedClear_Info()
{
	UpdateData(TRUE);
	Reset_Info();
	UpdateData(FALSE);
}


void CAnimationTool::OnTimer(UINT_PTR nIDEvent)
{

	CString wstrFindName;
	int i = 0;
	int iIndex = m_Animation_ListBox.GetCurSel();

	if (iIndex == LB_ERR)
		return;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);
	auto& iter_find = m_mapAnima.find(wstrFindName);

	if ((nIDEvent)<100)
	{
		SetImageView(iter_find->second->wstrObjectKey, m_iAnimationCount, m_Index_To_Selected_Image, iter_find->second->wstrStateKey);
		if (++m_iAnimationCount >= iter_find->second->iMax_Index)
			m_iAnimationCount = 0;
	}

	CDialog::OnTimer(nIDEvent);
}


void CAnimationTool::OnBnClickedPlay()
{
	UpdateData(TRUE);
	SetTimer(m_iTimer++, m_iPlay_Speed, NULL);
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedStop()
{
	KillTimer(m_iKillTimer++);
}
