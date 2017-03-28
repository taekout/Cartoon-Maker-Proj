// DrawDlg.cpp : implementation file
//

/*
      BOOL CMyView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
      {
            if ( m_ChangeCursor )
              {
                  ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
                  return TRUE;
              }

            return CView::OnSetCursor(pWnd, nHitTest, message);
      }


*/

#include "stdafx.h"
#include "CartoonMaker.h"
#include "DrawDlg.h"
#include "XmlParser.h"
#include "BitmapEx.h"
#include "CartoonMakerDoc.h"

// CDrawDlg dialog

IMPLEMENT_DYNAMIC(CDrawDlg, CDialog)

CDrawDlg::CDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawDlg::IDD, pParent)
{
	m_pFrame = NULL;
	m_pStaticBitmap = NULL;
	m_bSetPixel = FALSE;
	this ->m_color = RGB(255, 255, 255);
	m_bDrawLine = FALSE;
}

CDrawDlg::~CDrawDlg()
{
}

void CDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CDrawDlg, CDialog)
	ON_BN_CLICKED(IDC_DRAW, &CDrawDlg::OnBnClickedDrawLine)
	ON_BN_CLICKED(IDC_LUMINANCE, &CDrawDlg::OnBnClickedLuminance)
	ON_BN_CLICKED(IDC_SETCOLOR, &CDrawDlg::OnBnClickedSetcolor)
	ON_BN_CLICKED(IDC_BLUR, &CDrawDlg::OnBnClickedBlur)
	ON_BN_CLICKED(IDC_EMBOSS, &CDrawDlg::OnBnClickedEmboss)
	ON_BN_CLICKED(IDC_GREYSCALE, &CDrawDlg::OnBnClickedGreyscale)
	ON_BN_CLICKED(IDC_ROTATE, &CDrawDlg::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_PIXEL, &CDrawDlg::OnBnClickedPixel)
	ON_BN_CLICKED(IDC_SHARP, &CDrawDlg::OnBnClickedSharp)
	ON_BN_CLICKED(IDC_PEN, &CDrawDlg::OnBnClickedPen)
	ON_BN_CLICKED(IDC_FloodFill, &CDrawDlg::OnBnClickedFloodfill)
	ON_BN_CLICKED(IDC_SAVE, &CDrawDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_GetColor, &CDrawDlg::OnBnClickedGetcolor)
END_MESSAGE_MAP()


// CDrawDlg message handlers
// DrawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CartoonMaker.h"
#include "DrawDlg.h"


// CSliderDlg dialog

IMPLEMENT_DYNAMIC(CSliderDlg, CDialog)

CSliderDlg::CSliderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSliderDlg::IDD, pParent)
	, m_strStatic(_T(""))
{
	m_pDoc = NULL;
}

CSliderDlg::~CSliderDlg()
{
}

void CSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ROAD, m_sliderBus);
	DDX_Text(pDX, IDC_SLIDERSTATIC, m_strStatic);
}


BEGIN_MESSAGE_MAP(CSliderDlg, CDialog)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnSliderMoving)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, OnSliderMoved)
//	ON_STN_CLICKED(IDC_SLIDER_ROAD, &CSliderDlg::OnStnClickedSliderRoad)
//ON_STN_CLICKED(IDC_SLIDERSTATIC, &CSliderDlg::OnStnClickedSliderstatic)
//ON_STN_CLICKED(IDC_SLIDERSTATIC, &CSliderDlg::OnStnClickedSliderstatic)
ON_STN_CLICKED(IDC_SLIDERSTATIC, &CSliderDlg::OnStnClickedSliderstatic)
END_MESSAGE_MAP()


// CSliderDlg message handlers

BOOL CSliderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_sliderBus.SetBitmapChannel(IDB_SLIDERBAR, NULL, TRUE );
	m_sliderBus.SetBitmapThumb(IDB_SLIDERCURSOR, NULL, TRUE );
	m_sliderBus.SetRange( 0, 2000 );
	m_sliderBus.SetPos( 8 );
	m_sliderBus.SetMarginLeft( 8 );
	m_sliderBus.SetMarginRight( 8 );

	m_strStatic = _T("0");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CSliderDlg::OnSliderMoving(WPARAM wParam, LPARAM lParam)
{
	m_strStatic.Format(_T("%d"), lParam);
	UpdateData(FALSE);
	((CCartoonMakerDoc *)m_pDoc) ->cnt_point=0;
	((CCartoonMakerDoc *)m_pDoc) ->play_frame= (INT)lParam;
	((CCartoonMakerDoc *)m_pDoc) ->UpdateAllViews(NULL);
	return	0;
}

LRESULT CSliderDlg::OnSliderMoved(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDC_SLIDER_ROAD)
	{
		CString str;
		str.Format(_T("%d"),  lParam);

		m_strStatic = str;
		UpdateData(FALSE);
		((CCartoonMakerDoc *)m_pDoc) ->cnt_point=0;
		((CCartoonMakerDoc *)m_pDoc) ->play_frame= (INT)lParam;
		((CCartoonMakerDoc *)m_pDoc) ->UpdateAllViews(NULL);
//		AfxMessageBox(str.GetString());
	}
	return 0;
}


BOOL CSliderDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
//	SetWindowPos(this, 0, GetSystemMetrics(SM_CYFULLSCREEN) - 40,GetSystemMetrics(SM_CXFULLSCREEN), 40, 0);


	return CDialog::PreCreateWindow(cs);
}

BOOL CDrawDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreCreateWindow(cs);
}

BOOL CDrawDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	MoveWindow(0, GetSystemMetrics(SM_CYFULLSCREEN) / 32 * 3, WINDOW_START_METRICS, 550);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawDlg::OnBnClickedDrawLine()
{
	// TODO: Add your control notification handler code here
	if(m_bDrawLine == TRUE)
	{
		m_bDrawLine = FALSE;
		m_bSetPixel = FALSE;
	}
	else
	{
		m_bDrawLine = TRUE;
		m_bSetPixel = FALSE;
	}
}

void CDrawDlg::OnBnClickedLuminance()
{
	// TODO: Add your control notification handler code here
	CMenu *pMenu;
	CMenu *pSubmenu;
	pMenu = new CMenu();
	pMenu ->LoadMenu(IDR_EXTRAMENUS);
	pSubmenu = pMenu ->GetSubMenu(5);
	CPoint point(80,350);
	this ->ClientToScreen(&point);

	pSubmenu ->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	delete pMenu;
}

void CDrawDlg::OnBnClickedSetcolor()
{
	// TODO: Add your control notification handler code here
//	CDocument *pDoc;
	CColorDialog dlg(m_color);
	dlg.DoModal();
	m_color = dlg.GetColor();
}


void CDrawDlg::OnBnClickedBlur()
{
	// TODO: Add your control notification handler code here
	static BOOL bBlurred = FALSE;
	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	if(bBlurred == FALSE)
	{
		pBitmap ->Blur();
		bBlurred = TRUE;
	}
	else
	{
		bBlurred = FALSE;
	}


	pBitmap ->Draw(this ->m_pFrame ->GetActiveFrame() ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CDrawDlg::OnBnClickedEmboss()
{
	// TODO: Add your control notification handler code here
	static BOOL bEmbossed = FALSE;
	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	if(bEmbossed == FALSE)
	{
		pBitmap ->Emboss();
		bEmbossed = TRUE;
	}
	else
	{
		bEmbossed = FALSE;
	}

	pBitmap ->Draw(this ->m_pFrame ->GetActiveFrame() ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CDrawDlg::OnBnClickedGreyscale()
{
	// TODO: Add your control notification handler code here
	static BOOL bGreyscaled = FALSE;
	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	if(bGreyscaled == FALSE)
	{
		pBitmap ->Grayscale();
		bGreyscaled = TRUE;
	}
	else
		bGreyscaled = FALSE;

	pBitmap ->Draw(this ->m_pFrame ->GetActiveFrame() ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CDrawDlg::OnBnClickedRotate()
{
	// TODO: Add your control notification handler code here
	static BOOL bRotated = FALSE;
	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	if(bRotated == FALSE)
	{
		pBitmap ->Rotate(180);
		bRotated = TRUE;
	}
	else
	{
		bRotated = FALSE;
	}

	pBitmap ->Draw(this ->m_pFrame ->GetActiveFrame() ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CDrawDlg::OnBnClickedPixel()
{
	// TODO: Add your control notification handler code here
	if(m_bSetPixel == TRUE)
	{
		m_bSetPixel = FALSE;
		m_bDrawLine = FALSE;
	}
	else
	{
		m_bSetPixel = TRUE;
		m_bDrawLine = FALSE;
	}
}

void CDrawDlg::OnBnClickedSharp()
{
	// TODO: Add your control notification handler code here
	static BOOL bSharp = FALSE;
	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	if(bSharp == FALSE)
	{
		pBitmap ->Sharp();
		bSharp = TRUE;
	}
	else
	{
		bSharp = FALSE;
	}

	pBitmap ->Draw(this ->m_pFrame ->GetActiveFrame() ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CSliderDlg::OnStnClickedSliderstatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDrawDlg::OnBnClickedPen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCartoonMakerDoc * pDoc = (CCartoonMakerDoc *)m_pDoc;
	pDoc->cnt_point=0;
	pDoc->state_editor=1;
}

void CDrawDlg::OnBnClickedFloodfill()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCartoonMakerDoc * pDoc = (CCartoonMakerDoc *)m_pDoc;
	pDoc->state_editor=2;
}
void CDrawDlg::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i,j;
	CCartoonMakerDoc * pDoc = (CCartoonMakerDoc *)m_pDoc;
	

	pDoc->check_save=true;
	pDoc->UpdateAllViews(NULL);
}
void CDrawDlg::OnBnClickedGetcolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCartoonMakerDoc * pDoc = (CCartoonMakerDoc *)m_pDoc;
	pDoc->state_editor=0;
}
