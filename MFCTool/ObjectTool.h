#pragma once
#include "afxwin.h"


// CObjectTool 대화 상자입니다.

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void Update_Group_Bullet();
	void Update_Group_ShotGun();

public:
	map<CString, OBJECTINFO*>	m_mapObject;
	map<CString, CString>		m_mapKeyToPath;
public:
	afx_msg void OnCbnSelchangeBulletType();
	CStatic m_GroupBox;
	CComboBox m_BulletTypeSelectControl;
	afx_msg void OnBnClickedAdd();
	CString m_cstrName;
	CButton m_CheckBoxDestructable;
	int m_iShotGunCount;
	float m_fShotGunAngle;
	CListBox m_ListBox_ObjectList;
	CListBox m_ListBox_AnimList;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CStatic m_ObjectPicture;
	afx_msg void OnLbnSelchangeObjectList();
	afx_msg void OnBnClickedButtonObjectDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonLoadAnimation();
	afx_msg void OnLbnSelchangeAnimation();
	CStatic m_PictureAnimation;
	float m_fMaxHp;
	float m_fObjAtk;
	float m_fObjAtkRatio;
	float m_fObjMoveSpeed;
	CComboBox m_ComboOBJID;
	afx_msg void OnBnClickedButtonObjPlaystop();

private:
	bool	m_bIsPlaying_DeathAnimation;
	bool	m_bIsPlaying_Idle;
	bool	m_bIsPlaying_Crash;


	int		m_iDeathAnimationIndex;
	int		m_iIdleIndex;
	int		m_iCrashIndex;


	bool m_bIdle;
	bool m_bDeadEff;
	bool m_bCrash;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_ButtonAnimationPlay;
	afx_msg void OnCbnSelchangeComboObjid();
	afx_msg void OnLbnSelchangeListObjpic();
	CStatic m_PictureObject;
	CListBox m_ListBox_ObjImage;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelIdle();
	afx_msg void OnLbnSelCrash();
	afx_msg void OnBnClickedAddIdle();
	afx_msg void OnBnClickedButtonAddCrash();
	afx_msg void OnBnClickedButtoIdlePlay();
	afx_msg void OnBnClickedButtonCrashPlay();
	CListBox m_IdleListBox;
	CListBox m_CrashListBox;
	CStatic m_IdlePicture;
	CStatic m_CrashPicture;
	CButton m_ButtonIdlePlay;
	CButton m_ButtonCrash;
};
