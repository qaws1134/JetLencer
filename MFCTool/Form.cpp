// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Form.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON6, &CForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON9, &CForm::OnBnClickedUiTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CForm::OnBnClickedAnimationTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CForm::OnBnClickedBulletTool)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.

void CForm::OnInitialUpdate() // 이것도 내가 재정의 함. 
{
	CFormView::OnInitialUpdate();
	if (nullptr == m_tMapTool.GetSafeHwnd())
		m_tMapTool.Create(IDD_MAPTOOL);
	if (nullptr == m_tAnimationTool.GetSafeHwnd())
		m_tAnimationTool.Create(IDD_ANIMATIONTOOL);
	if (nullptr == m_tObjectTool.GetSafeHwnd())
		m_tObjectTool.Create(IDD_OBJECTTOOL);
	if (nullptr == m_tUiTool.GetSafeHwnd())
		m_tUiTool.Create(IDD_UITOOL);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

// MapTool 
void CForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_tMapTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedUiTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_tUiTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedAnimationTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_tAnimationTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedBulletTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_tObjectTool.ShowWindow(SW_SHOW);
}
