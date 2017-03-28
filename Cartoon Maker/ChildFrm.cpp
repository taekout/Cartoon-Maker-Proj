// ChildFrm.cpp : CChildFrame Ŭ������ ����
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


// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame ����

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


// CChildFrame �޽��� ó����
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
