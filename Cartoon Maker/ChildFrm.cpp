// ChildFrm.cpp : CChildFrame 클래스의 구현
//
#include "stdafx.h"
#include "CartoonMaker.h"
#include "mainfrm.h"
#include "ChildFrm.h"
#include "CartoonMakerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_MESSAGE(WM_WINDOW_SIZE_FOR_PICTURE , AdjustWindowSize)
END_MESSAGE_MAP()


// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame 메시지 처리기
LRESULT CChildFrame::AdjustWindowSize(WPARAM wParam, LPARAM lParam)
{
//	WPARAM the highest 8 bits == Width
//	
//	lParam the lowest 8 bits == Height
//	MoveWindow(WINDOW_START_METRICS ,0, (INT)wParam, (INT)lParam+30);
	CRect rt;
	::AfxGetMainWnd() ->GetWindowRect(&rt);

	SetWindowPos(&CWnd::wndTop, (rt.left + rt.right - (INT)wParam) / 2,
		(rt.top + rt.bottom - (INT)lParam) / 2 - 100,
		(INT)wParam, (INT)lParam + 38, SWP_FRAMECHANGED);

	return	0;
}
