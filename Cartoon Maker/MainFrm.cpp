// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "CartoonMaker.h"
#include "CartoonMakerDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_WINDOW_SIZE_FOR_PICTURE, AdjustWindowSize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_pManagerDlg = NULL;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if(m_pManagerDlg == NULL)
	{
		m_pManagerDlg = new CManagerDlg();
		m_pManagerDlg ->Create(IDD_FILEMANAGER);
		m_pManagerDlg ->ShowWindow(SW_SHOW);
	}
	CMenu *pMainmenu = this ->GetMenu();
	pMainmenu ->EnableMenuItem(ID_AVICONTROL_CARTOONMAKING, TRUE);
//	pMainmenu ->EnableMenuItem(ID_FILE_CAM, TRUE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style |= WS_MAXIMIZE;
	CWaitCursor wait;

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기



LRESULT CMainFrame::AdjustWindowSize(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(this ->GetActiveFrame() ->GetSafeHwnd(), WM_WINDOW_SIZE_FOR_PICTURE,
		wParam, lParam);

	return	0;
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}


/*
	CXmlParser pa;
	CString xmldoc;
	xmldoc = pa.LoadXmlFile(FILE_DATABASE_NAME);
	if(xmldoc.CompareNoCase(_T("unable to open file")) == 0 )
		return FALSE;
	CString tmp;
	while(pa.FindChildElem(_T("Video")))
	{
		tmp = pa.GetAttrib(_T("videoFileName"));
		// Here!!! Problem!!! The document point is NULL. has never been initialized.
		((CCartoonMakerDoc *)this ->GetActiveDocument()) ->OnOpenDocument(tmp);
	}

*/