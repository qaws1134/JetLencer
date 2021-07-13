
// MFCToolView.cpp : CMFCToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "Single_Texture.h"
#include "Terrain.h"
#include "MainFrm.h"
#include "Miniview.h"
#include "MapTool.h"
#include "Form.h"
#include "UiTool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HWND g_hWND; 

// CMFCToolView

IMPLEMENT_DYNCREATE(CMFCToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
	:m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolView::~CMFCToolView()
{

	Safe_Delete(m_pTerrain);
	CTexture_Manager::Destroy_Instance(); 
	CGraphic_Device::Destroy_Instance(); 
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// 그림을 그리기 위한 일련의 과정 
	//1. 백버퍼를 비운다 
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	//m_pTerrain->Render_Terrain(); 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniview* pView = dynamic_cast<CMiniview*>(pMain->m_tSecondSplitter.GetPane(0, 0));
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	pForm->m_tUiTool.SetView(this);
	if (pForm->m_tUiTool.Get_Multi())
	{
		pForm->m_tUiTool.Render_MultiUI();
	}
	else
	{
		pForm->m_tUiTool.Render_UI();
	}
	CGraphic_Device::Get_Instance()->Render_End();
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SIZE; 
	//CSize tsize; 
	SetScrollSizes(MM_TEXT, CSize(Map_Width,Map_Height));
	//GetScrollPos()
	g_hWND = m_hWnd; 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd()); 
	RECT rcMain = {}; 
	pMain->GetWindowRect(&rcMain);

	RECT rcView{}; 
	GetClientRect(&rcView);  

// 	rcMain.right = rcMain.right - rcMain.left; 
// 	rcMain.bottom = rcMain.bottom - rcMain.top; 
// 	rcMain.left = 0; 
// 	rcMain.right = 0; 
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	int iGapX = rcMain.right - rcView.right; 
	int iGapY = rcMain.bottom - rcView.bottom; 

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOMOVE);

	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
		return; 

}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
// 	TCHAR szBuf[54] = L""; 
// 	swprintf_s(szBuf, L"X : %d , Y : %d", point.x, point.y); 
// 	ERR_MSG(szBuf); 
	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f }; 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniview* pView = dynamic_cast<CMiniview*>(pMain->m_tSecondSplitter.GetPane(0, 0));
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));


	pForm->m_tUiTool.PickingPos(vMouse);
	//여기서 충돌 체크 
	pForm->m_tUiTool.Collision_Down(vMouse);
	pView->Invalidate(FALSE); 

	Invalidate(FALSE); 
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMFCToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniview* pView = dynamic_cast<CMiniview*>(pMain->m_tSecondSplitter.GetPane(0, 0));
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));

	pForm->m_tUiTool.Collision_Move(vMouse);

	CScrollView::OnMouseMove(nFlags, point);
}


void CMFCToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniview* pView = dynamic_cast<CMiniview*>(pMain->m_tSecondSplitter.GetPane(0, 0));
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));

	pForm->m_tUiTool.Collision_Up();
	CScrollView::OnLButtonUp(nFlags, point);
}
