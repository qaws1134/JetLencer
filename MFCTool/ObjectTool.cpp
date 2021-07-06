// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"


// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_cstrName(_T(""))
	, m_iShotGunCount(0)
	, m_fShotGunAngle(0.f)
	, m_fMaxHp(0.f)
	, m_fObjAtk(0.f)
	, m_fObjAtkRatio(0.f)
	, m_fObjMoveSpeed(0.f)
	, m_bIsPlaying_DeathAnimation(false)
	, m_iDeathAnimationIndex(0)
	, m_bDeadEff(false)
	, m_bIdle(false)
	, m_bCrash(false)
	,m_bIsPlaying_Idle(false)
	, m_bIsPlaying_Crash(false)
	,m_iIdleIndex(0)
	,m_iCrashIndex(0)

{

}

CObjectTool::~CObjectTool()
{
	for (auto& rPair : m_mapObject)
		Safe_Delete(rPair.second);
	m_mapObject.clear();
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_SG, m_GroupBox);
	DDX_Control(pDX, IDC_COMBO_BULLET_TYPE, m_BulletTypeSelectControl);
	DDX_Text(pDX, IDC_EDIT_OBJNAME, m_cstrName);
	DDX_Control(pDX, IDC_CHECK1_BULLET, m_CheckBoxDestructable);
	DDX_Text(pDX, IDC_EDIT_COUNT_SG, m_iShotGunCount);
	DDX_Text(pDX, IDC_EDIT_ANGLE_SG, m_fShotGunAngle);
	DDX_Control(pDX, IDC_LIST2_OBJ, m_ListBox_ObjectList);
	DDX_Control(pDX, IDC_LIST3_OBJ, m_ListBox_AnimList);
	DDX_Control(pDX, IDC_ANIM_PIC, m_PictureAnimation);
	DDX_Text(pDX, IDC_EDIT_OBJHP, m_fMaxHp);
	DDX_Text(pDX, IDC_EDIT_OBJATK, m_fObjAtk);
	DDX_Text(pDX, IDC_EDIT_OBJATKSPD, m_fObjAtkRatio);
	DDX_Text(pDX, IDC_EDIT_OBJMOVSPD, m_fObjMoveSpeed);
	DDX_Control(pDX, IDC_COMBO_OBJID, m_ComboOBJID);
	DDX_Control(pDX, IDC_BUTTON_OBJ_PLAYSTOP, m_ButtonAnimationPlay);
	DDX_Control(pDX, IDC_PIC_OBJECT, m_PictureObject);
	DDX_Control(pDX, IDC_LIST_OBJPIC, m_ListBox_ObjImage);
	DDX_Control(pDX, IDC_LIST3_OBJ2, m_IdleListBox);
	DDX_Control(pDX, IDC_LIST3_OBJ3, m_CrashListBox);
	DDX_Control(pDX, IDC_ANIM_PIC2, m_IdlePicture);
	DDX_Control(pDX, IDC_ANIM_PIC3, m_CrashPicture);
	DDX_Control(pDX, IDC_BUTTON_OBJ_PLAYSTOP2, m_ButtonIdlePlay);
	DDX_Control(pDX, IDC_BUTTON_OBJ_PLAYSTOP3, m_ButtonCrash);
}

void CObjectTool::Update_Group_Bullet()
{
	int iIndex = m_ComboOBJID.GetCurSel();

	if (iIndex == OBJID::ENEMY_BULLET || iIndex == OBJID::PLAYER_BULLET)
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_NORMAL);
	}
	else
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_HIDE);
	}
	Update_Group_ShotGun();
}

void CObjectTool::Update_Group_ShotGun()
{
	int iIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iIndex == CB_ERR)
	{
		return;
	}
	CString strType;
	CString strShotgun = _T("2산탄");
	m_BulletTypeSelectControl.GetLBText(iIndex, strType);
	if (strType == strShotgun)
	{
		m_GroupBox.ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_ANGLE_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_COUNT_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_ANGLE_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_COUNT_SG)->ShowWindow(SW_NORMAL);
	}
	else
	{
		m_GroupBox.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_ANGLE_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_COUNT_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ANGLE_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COUNT_SG)->ShowWindow(SW_HIDE);
	}
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_BULLET_TYPE, &CObjectTool::OnCbnSelchangeBulletType)
	ON_BN_CLICKED(IDC_BUTTON_OBJADD, &CObjectTool::OnBnClickedAdd)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST2_OBJ, &CObjectTool::OnLbnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON_OBJDEL, &CObjectTool::OnBnClickedButtonObjectDelete)
	ON_BN_CLICKED(IDC_BUTTON_OBJSAVE, &CObjectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOAD, &CObjectTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOADANIM, &CObjectTool::OnBnClickedButtonLoadAnimation)
	ON_LBN_SELCHANGE(IDC_LIST3_OBJ, &CObjectTool::OnLbnSelchangeAnimation)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_PLAYSTOP, &CObjectTool::OnBnClickedButtonObjPlaystop)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_OBJID, &CObjectTool::OnCbnSelchangeComboObjid)
	ON_LBN_SELCHANGE(IDC_LIST_OBJPIC, &CObjectTool::OnLbnSelchangeListObjpic)
	ON_LBN_SELCHANGE(IDC_LIST3_OBJ2, &CObjectTool::OnLbnSelIdle)
	ON_LBN_SELCHANGE(IDC_LIST3_OBJ3, &CObjectTool::OnLbnSelCrash)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOADANIM2, &CObjectTool::OnBnClickedAddIdle)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOADANIM3, &CObjectTool::OnBnClickedButtonAddCrash)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_PLAYSTOP2, &CObjectTool::OnBnClickedButtoIdlePlay)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_PLAYSTOP3, &CObjectTool::OnBnClickedButtonCrashPlay)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.


void CObjectTool::OnCbnSelchangeBulletType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_Group_ShotGun();
}


//Bullet Add
void CObjectTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	if (m_cstrName.IsEmpty())
	{
		ERR_MSG(L"이름을 채워넣으세요");
		return;
	}

	int iObjID = m_ComboOBJID.GetCurSel();
	if (iObjID == CB_ERR)
	{
		ERR_MSG(L"오브젝트 타입을 선택해주세요");
		return;
	}

	int iImage = m_ListBox_ObjImage.GetCurSel();


	int iDeathAnimationIndex = m_ListBox_AnimList.GetCurSel();
	if (iDeathAnimationIndex == LB_ERR&& m_bDeadEff)
	{
		ERR_MSG(L"사망 애니메이션을 선택해주세요");
		return;
	}


	int iIdleAniIndex = m_IdleListBox.GetCurSel();
	if (iIdleAniIndex == LB_ERR && m_bIdle)
	{
		ERR_MSG(L"Idle 애니메이션을 선택해주세요");
		return;
	}

	int iCrashAniIndex = m_CrashListBox.GetCurSel();
	if (iCrashAniIndex == LB_ERR&& m_bCrash)
	{
		ERR_MSG(L"Crash 애니메이션을 선택해주세요");
		return;
	}


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	CString cstrName;

	int iBulletTypeIndex = m_BulletTypeSelectControl.GetCurSel();
	if (iBulletTypeIndex == CB_ERR)
		iBulletTypeIndex = 0;

	OBJECTINFO*	pObjectData = new OBJECTINFO;
	pObjectData->cstrName = m_cstrName;
	pObjectData->fMaxHp = m_fMaxHp;
	pObjectData->fAtk = m_fObjAtk;
	pObjectData->fAtkRatio = m_fObjAtkRatio;
	pObjectData->fMoveSpeed = m_fObjMoveSpeed;
	pObjectData->eObjId = (OBJID::ID)iObjID;
	if (iImage == LB_ERR)
	{
		pObjectData->cstrObjectImage_ObjectKey = L"";
		pObjectData->cstrObjectImage_Path = L"";
	}
	else
	{
		m_ListBox_ObjImage.GetText(iImage, cstrName);
		auto& iter_find = m_mapKeyToPath.find(cstrName);
		if (iter_find == m_mapKeyToPath.end())
		{
			pObjectData->cstrObjectImage_ObjectKey = L"";
			pObjectData->cstrObjectImage_Path = L"";
		}
		else
		{
			pObjectData->cstrObjectImage_ObjectKey = cstrName;
			pObjectData->cstrObjectImage_Path = iter_find->second;
		}
		cstrName = L"";
	}

	if (m_bDeadEff)
	{
		m_ListBox_AnimList.GetText(iDeathAnimationIndex, cstrName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < cstrName.GetLength(); ++i)
		{
			if (cstrName[i] == L'>')
				break;
		}
		cstrName.Delete(i - 1, 2);
		auto& iter_find_Dead = map.find(cstrName);
		if (iter_find_Dead == map.end() && m_bDeadEff)
		{
			ERR_MSG(L"애니메이션을 찾지 못했습니다.");
			return;
		}
		pObjectData->cstrDeathAnimImage_ObjectKey = iter_find_Dead->second->wstrObjectKey;
		pObjectData->cstrDeathAnimImage_StateKey = iter_find_Dead->second->wstrStateKey;
		m_ListBox_AnimList.ResetContent();
		m_bDeadEff = false;
	}
	if (m_bIdle)
	{
		cstrName = L"";

		m_IdleListBox.GetText(iIdleAniIndex, cstrName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < cstrName.GetLength(); ++i)
		{
			if (cstrName[i] == L'>')
				break;
		}
		cstrName.Delete(i - 1, 2);
		auto& iter_find_Idle = map.find(cstrName);
		if (iter_find_Idle == map.end() && m_bIdle)
		{
			ERR_MSG(L"애니메이션을 찾지 못했습니다.");
			return;
		}
		pObjectData->cstrIdleImage_ObjectKey = iter_find_Idle->second->wstrObjectKey;
		pObjectData->cstrIdleImage_StateKey= iter_find_Idle->second->wstrStateKey;
		m_IdleListBox.ResetContent();
		m_bIdle = false;
	}
	if (m_bCrash)
	{
		cstrName = L"";

		m_IdleListBox.GetText(iCrashAniIndex, cstrName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < cstrName.GetLength(); ++i)
		{
			if (cstrName[i] == L'>')
				break;
		}
		cstrName.Delete(i - 1, 2);
		auto& iter_find_Crash = map.find(cstrName);
		if (iter_find_Crash == map.end() && m_bCrash)
		{
			ERR_MSG(L"애니메이션을 찾지 못했습니다.");
			return;
		}
		pObjectData->cstrCrashImage_ObjectKey = iter_find_Crash->second->wstrObjectKey;
		pObjectData->cstrCrashImage_StateKey= iter_find_Crash->second->wstrStateKey;
		m_CrashListBox.ResetContent();
		m_bCrash = false;
	}

	//Bullet Data	
	pObjectData->bDestructable = (bool)m_CheckBoxDestructable.GetCheck();
	pObjectData->eBulletType = (BULLET::TYPE)iBulletTypeIndex;

	//ShotGun Data
	if (pObjectData->eBulletType == BULLET::SHOTGUN)
	{
		pObjectData->iShotGunCount = m_iShotGunCount;
		pObjectData->fShotGunAngle = m_fShotGunAngle;
	}
	else
	{
		pObjectData->iShotGunCount = 0;
		pObjectData->fShotGunAngle = 0.f;
	}


	auto& iter = m_mapObject.find(m_cstrName);
	if (iter != m_mapObject.end())
	{
		Safe_Delete(iter->second);
		m_mapObject.erase(iter);
	}
	else
	{
		m_ListBox_ObjectList.AddString(m_cstrName);
	}
		

	m_mapObject.emplace(m_cstrName, pObjectData); // Add는 수정도 가능

}


void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	int iDropCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};
	
	for (int i = 0; i < iDropCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);
		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		auto& iter_find = m_mapKeyToPath.find(szFileName);
		if (iter_find != m_mapKeyToPath.end())
			continue;
		m_ListBox_ObjImage.AddString(szFileName);
		CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrRelativePath.GetString(), szFileName);
		m_mapKeyToPath.emplace(szFileName, wstrRelativePath);
	}
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

void CObjectTool::OnLbnSelchangeObjectList()
{
	int iObjectIndex = m_ListBox_ObjectList.GetCurSel();

	CString cstrName;
	m_ListBox_ObjectList.GetText(iObjectIndex, cstrName);

	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
		return;

	OBJECTINFO* pObjectData = iter->second;
		
	m_cstrName = pObjectData->cstrName;
	m_fMaxHp = pObjectData->fMaxHp;
	m_fObjAtk = pObjectData->fAtk;
	m_fObjAtkRatio = pObjectData->fAtkRatio;
	m_fObjMoveSpeed = pObjectData->fMoveSpeed;
	m_ComboOBJID.SetCurSel(pObjectData->eObjId);

	if (m_bDeadEff)
	{
		OnBnClickedButtonLoadAnimation();
		int findIndex = m_ListBox_AnimList.FindStringExact(-1, pObjectData->cstrDeathAnimImage_ObjectKey + L"->" + pObjectData->cstrDeathAnimImage_StateKey);
		if (findIndex != -1)
		{
			m_ListBox_AnimList.SetCurSel(findIndex);
			OnLbnSelchangeAnimation();
		}
	}
		
	if (m_bIdle)
	{
		OnBnClickedAddIdle();
		int findIndex = m_IdleListBox.FindStringExact(-1, pObjectData->cstrIdleImage_ObjectKey + L"->" + pObjectData->cstrIdleImage_StateKey);
		if (findIndex != -1)
		{
			m_IdleListBox.SetCurSel(findIndex);
			OnLbnSelIdle();
		}
	}

	if (m_bCrash)
	{
		OnBnClickedButtonAddCrash();
		int findIndex = m_CrashListBox.FindStringExact(-1, pObjectData->cstrCrashImage_ObjectKey + L"->" + pObjectData->cstrCrashImage_StateKey);
		if (findIndex != -1)
		{
			m_CrashListBox.SetCurSel(findIndex);
			OnLbnSelCrash();
		}
	}
	int findIndex = m_ListBox_ObjImage.FindStringExact(-1, pObjectData->cstrObjectImage_ObjectKey);
	if (findIndex != -1)
	{
		m_ListBox_ObjImage.SetCurSel(findIndex);
		OnLbnSelchangeListObjpic();
	}
		

	m_BulletTypeSelectControl.SetCurSel(pObjectData->eBulletType);
	m_CheckBoxDestructable.SetCheck(pObjectData->bDestructable);

	Update_Group_Bullet();
	Update_Group_ShotGun();
	if (pObjectData->eBulletType == BULLET::SHOTGUN)
	{
		m_iShotGunCount = pObjectData->iShotGunCount;
		m_fShotGunAngle = pObjectData->fShotGunAngle;
	}
	else
	{
		m_iShotGunCount = 0;
		m_fShotGunAngle = 0.f;
	}

	UpdateData(FALSE);
}


void CObjectTool::OnBnClickedButtonObjectDelete()
{
	int iIndex = m_ListBox_ObjectList.GetCurSel();
	
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"삭제할 대상을 선택해 주세요");
		return;
	}

	CString cstrName;
	m_ListBox_ObjectList.GetText(iIndex, cstrName);
	
	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
	{
		ERR_MSG(L"ListBox에는 존재하지만 Map에는 존재하지 않는 키값입니다");
		return;
	}
	
	Safe_Delete(iter->second);
	m_mapObject.erase(iter);

	m_ListBox_ObjectList.DeleteString(iIndex);
}


void CObjectTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"ObjectData.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	//PathCombine(szFilePath, szFilePath, L"Data"); 
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
		//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;
		DWORD strLen;
		// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
		for (auto& rPair : m_mapObject)
		{
			OBJECTINFO* pObject = rPair.second;
			strLen = pObject->cstrName.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrName.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrObjectImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrObjectImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrObjectImage_Path.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrObjectImage_Path.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrDeathAnimImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrDeathAnimImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrDeathAnimImage_StateKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrDeathAnimImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);



			strLen = pObject->cstrIdleImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrIdleImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrIdleImage_StateKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrIdleImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrCrashImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrCrashImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrCrashImage_StateKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrCrashImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);


			WriteFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CObjectTool::OnBnClickedButtonLoad()
{
	OnBnClickedButtonLoadAnimation();
	OnBnClickedButtonAddCrash();
	OnBnClickedAddIdle();

	if (m_ListBox_AnimList.GetCount() <= 0)
	{
		ERR_MSG(L"애니메이션툴을 먼저 로드해주세요");
		return;
	}

	CFileDialog Dlg(TRUE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	//PathCombine(szFilePath, szFilePath, L"Data"); 
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
		//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		for (auto& rPair : m_mapObject)
		{
			Safe_Delete(rPair.second);
		}
		m_mapObject.clear();
		m_mapKeyToPath.clear();
		m_ListBox_ObjectList.ResetContent();
		m_ListBox_ObjImage.ResetContent();

		DWORD dwbyte = 0;
		DWORD strLen;
		OBJECTINFO* pObject;
		TCHAR *pBuff;
		// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
		while (true)
		{
			
			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

			if (dwbyte == 0)
				break;

			pObject = new OBJECTINFO;

			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrName = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrObjectImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrObjectImage_Path = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrDeathAnimImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrDeathAnimImage_StateKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrIdleImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrIdleImage_StateKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrCrashImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrCrashImage_StateKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);

			m_mapObject.emplace(pObject->cstrName, pObject);

			m_ListBox_ObjectList.AddString(pObject->cstrName);
			if (!pObject->cstrObjectImage_ObjectKey.IsEmpty())
			{
				m_mapKeyToPath.emplace(pObject->cstrObjectImage_ObjectKey, pObject->cstrObjectImage_Path);
				m_ListBox_ObjImage.AddString(pObject->cstrObjectImage_ObjectKey);
				CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pObject->cstrObjectImage_Path.GetString(), pObject->cstrObjectImage_ObjectKey.GetString());
			}
		}
		CloseHandle(hFile);
	}
}


void CObjectTool::OnBnClickedButtonLoadAnimation()
{
	if (!m_bDeadEff)
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		for (auto& rPair : map)
		{
			CString cstrTemp = rPair.second->wstrObjectKey + L"->" + rPair.second->wstrStateKey;
			if (m_ListBox_AnimList.FindStringExact(-1, cstrTemp.GetString()) == -1)
				m_ListBox_AnimList.AddString(cstrTemp);
		}
		m_ListBox_AnimList.SetHorizontalExtent(500);
		m_bDeadEff = true;
	}
	else
	{
		m_ListBox_AnimList.ResetContent();
		m_bDeadEff = false;
	}
	//Animation Map 로드코드 구간
}


void CObjectTool::OnLbnSelchangeAnimation()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString wstrFindName;
	m_ListBox_AnimList.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->wstrObjectKey.GetString(), iter_find->second->wstrStateKey.GetString(), m_iDeathAnimationIndex);
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

	CGraphic_Device::Get_Instance()->Render_End(m_PictureAnimation.m_hWnd);
}









void CObjectTool::OnBnClickedButtonObjPlaystop()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	if (iIndex == LB_ERR)
		return;

	if (!m_bIsPlaying_DeathAnimation)
	{
		m_bIsPlaying_DeathAnimation = true;
		SetTimer(1000, 0 ,NULL);
		m_ButtonAnimationPlay.SetWindowText(L"Stop");
	}
	else
	{
		m_bIsPlaying_DeathAnimation = false;
		KillTimer(1000);
		m_ButtonAnimationPlay.SetWindowText(L"Play");
	}
}


void CObjectTool::OnTimer(UINT_PTR nIDEvent)
{
	if (!(nIDEvent == 1000
		|| nIDEvent == 800
		|| nIDEvent == 900))
		return;


	if (m_bDeadEff)
	{
		int iIndex = m_ListBox_AnimList.GetCurSel();
		CString wstrFindName;
		m_ListBox_AnimList.GetText(iIndex, wstrFindName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < wstrFindName.GetLength(); ++i)
		{
			if (wstrFindName[i] == L'>')
				break;
		}
		wstrFindName.Delete(i - 1, 2);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		auto& iter_find = map.find(wstrFindName);

		iter_find->second->fPlay_Speed;
		++m_iDeathAnimationIndex;
		if (m_iDeathAnimationIndex >= iter_find->second->iMax_Index)
			m_iDeathAnimationIndex = 0;

		OnLbnSelchangeAnimation();
		SetTimer(1000, iter_find->second->fPlay_Speed * 1000.f, NULL);
	}
	if (m_bIdle)
	{
		int iIndex = m_IdleListBox.GetCurSel();
		CString wstrFindName;
		m_IdleListBox.GetText(iIndex, wstrFindName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < wstrFindName.GetLength(); ++i)
		{
			if (wstrFindName[i] == L'>')
				break;
		}
		wstrFindName.Delete(i - 1, 2);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		auto& iter_find = map.find(wstrFindName);

		iter_find->second->fPlay_Speed;
		++m_iIdleIndex;
		if (m_iIdleIndex >= iter_find->second->iMax_Index)
			m_iIdleIndex = 0;

		OnLbnSelIdle();
		SetTimer(900, iter_find->second->fPlay_Speed * 1000.f, NULL);
	}
	if (m_bCrash)
	{
		int iIndex = m_CrashListBox.GetCurSel();
		CString wstrFindName;
		m_CrashListBox.GetText(iIndex, wstrFindName);
		int i = 0;
		// "->" 문자 삭제
		for (; i < wstrFindName.GetLength(); ++i)
		{
			if (wstrFindName[i] == L'>')
				break;
		}
		wstrFindName.Delete(i - 1, 2);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		auto& iter_find = map.find(wstrFindName);

		iter_find->second->fPlay_Speed;
		++m_iCrashIndex;
		if (m_iCrashIndex >= iter_find->second->iMax_Index)
			m_iCrashIndex = 0;

		OnLbnSelCrash();
		SetTimer(800, iter_find->second->fPlay_Speed * 1000.f, NULL);
	}
	CDialog::OnTimer(nIDEvent);

}


void CObjectTool::OnCbnSelchangeComboObjid()
{
	Update_Group_Bullet();
}


void CObjectTool::OnLbnSelchangeListObjpic()
{
	int iIndex = m_ListBox_ObjImage.GetCurSel();

	if (iIndex == LB_ERR)
	{
		return;
	}

	CString cstrName;
	m_ListBox_ObjImage.GetText(iIndex, cstrName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(cstrName.GetString());
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / pTexInfo->tImageInfo.Width, WINCX / pTexInfo->tImageInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_PictureObject.m_hWnd);
}


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComboOBJID.ResetContent();
	m_ComboOBJID.AddString(L"0BackGround");
	m_ComboOBJID.AddString(L"1Player");
	m_ComboOBJID.AddString(L"2Monster");
	m_ComboOBJID.AddString(L"3Player_Bullet");
	m_ComboOBJID.AddString(L"4Enemy_Bullet");
	m_ComboOBJID.AddString(L"5Effect");
	m_ComboOBJID.AddString(L"6Ui");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectTool::OnLbnSelIdle()
{
	int iIndex = m_IdleListBox.GetCurSel();

	CString wstrFindName;
	m_IdleListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->wstrObjectKey.GetString(), iter_find->second->wstrStateKey.GetString(), m_iIdleIndex);
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

	CGraphic_Device::Get_Instance()->Render_End(m_IdlePicture.m_hWnd);
}


void CObjectTool::OnLbnSelCrash()
{
	int iIndex = m_CrashListBox.GetCurSel();

	CString wstrFindName;
	m_CrashListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" 문자 삭제
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->wstrObjectKey.GetString(), iter_find->second->wstrStateKey.GetString(), m_iCrashIndex);
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

	CGraphic_Device::Get_Instance()->Render_End(m_CrashPicture.m_hWnd);
}


void CObjectTool::OnBnClickedAddIdle()
{
	if (!m_bIdle)
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		for (auto& rPair : map)
		{
			CString cstrTemp = rPair.second->wstrObjectKey + L"->" + rPair.second->wstrStateKey;
			if (m_IdleListBox.FindStringExact(-1, cstrTemp.GetString()) == -1)
				m_IdleListBox.AddString(cstrTemp);
		}
		m_IdleListBox.SetHorizontalExtent(500);
		m_bIdle = true;
	}
	else
	{
		m_bIdle = false;
		m_IdleListBox.ResetContent();
	}
}


void CObjectTool::OnBnClickedButtonAddCrash()
{
	if (!m_bCrash)
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

		for (auto& rPair : map)
		{
			CString cstrTemp = rPair.second->wstrObjectKey + L"->" + rPair.second->wstrStateKey;
			if (m_CrashListBox.FindStringExact(-1, cstrTemp.GetString()) == -1)
				m_CrashListBox.AddString(cstrTemp);
		}
		m_CrashListBox.SetHorizontalExtent(500);
		m_bCrash = true;
	}
	else
	{
		m_bCrash = false;
		m_CrashListBox.ResetContent();
	}
}


void CObjectTool::OnBnClickedButtoIdlePlay()
{
	int iIndex = m_IdleListBox.GetCurSel();

	if (iIndex == LB_ERR)
		return;
	if (!m_bIsPlaying_Idle)
	{
		m_bIsPlaying_Idle = true;
		SetTimer(900, 0, NULL);
		m_ButtonIdlePlay.SetWindowText(L"Stop");
	}
	else
	{
		m_bIsPlaying_Idle = false;
		KillTimer(900);
		m_ButtonIdlePlay.SetWindowText(L"Play");
	}
}


void CObjectTool::OnBnClickedButtonCrashPlay()
{
	int iIndex = m_CrashListBox.GetCurSel();
	if (iIndex == LB_ERR)
		return;
	if (!m_bIsPlaying_Crash)
	{
		m_bIsPlaying_Crash = true;
		SetTimer(800, 0, NULL);
		m_ButtonCrash.SetWindowText(L"Stop");
	}
	else
	{
		m_bIsPlaying_Crash = false;
		KillTimer(800);
		m_ButtonCrash.SetWindowText(L"Play");
	}
}
