// Miniview.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Miniview.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "Terrain.h"

// CMiniview

IMPLEMENT_DYNCREATE(CMiniview, CView)

CMiniview::CMiniview()
{

}

CMiniview::~CMiniview()
{
}

BEGIN_MESSAGE_MAP(CMiniview, CView)
END_MESSAGE_MAP()


// CMiniview 그리기입니다.

void CMiniview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	// SplitterWnd -> Getpane
	// MainApp -> MainFrame -> MainSplitterWnd 얻어올수 있고 -> MFCToolView -> Terrain 

	CGraphic_Device::Get_Instance()->Render_End(m_hWnd); 
}


// CMiniview 진단입니다.

#ifdef _DEBUG
void CMiniview::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniview 메시지 처리기입니다.
