// CartoonMakerView.cpp : CCartoonMakerView 클래스의 구현
//

#include "stdafx.h"
#include "CartoonMaker.h"
#include "BitmapEx.h"
#include "CartoonMakerDoc.h"
#include "CartoonMakerView.h"
#include "images.h"
#include "afxwin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCartoonMakerView

IMPLEMENT_DYNCREATE(CCartoonMakerView, CView)

BEGIN_MESSAGE_MAP(CCartoonMakerView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AVI_LINE, &CCartoonMakerView::OnAviLine)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AVI_CARTOONMAKING, &CCartoonMakerView::OnAviCartoonmaking)
	ON_COMMAND(ID_AVI_ONEFRAME_CARTOONMAKING, &CCartoonMakerView::OnConvertOneFrameToManga)
	ON_COMMAND(ID_FILE_SAVE, &CCartoonMakerView::OnFileSave)
END_MESSAGE_MAP()

// CCartoonMakerView 생성/소멸

CCartoonMakerView::CCartoonMakerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CCartoonMakerView::~CCartoonMakerView()
{
}

BOOL CCartoonMakerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.


	return CView::PreCreateWindow(cs);
}

// CCartoonMakerView 그리기
void CCartoonMakerView::OnDraw(CDC* pDC)
{
	CCartoonMakerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CString filetype = pDoc ->m_filename.Right(3);
	if(filetype.CompareNoCase(_T("AVI")) == 0)
	{
		int i,j;
		CvScalar f;
		CDC memDC;
		CBitmap* OldBitmap,Bitmap;
		CRect rect;
		this->GetClientRect(&rect);
		Bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
		memDC.CreateCompatibleDC(pDC);
		OldBitmap=memDC.SelectObject(&Bitmap);
		memDC.PatBlt(0,0,rect.Width(),rect.Height(),WHITENESS);

		if (pDoc->cnt_frame>pDoc->play_frame){
			for(i=0;i<pDoc->frame[pDoc->play_frame]->height;i++){
				for(j=0;j<pDoc->frame[pDoc->play_frame]->width;j++){
					memDC.SetPixel(j,i,RGB(
						pDoc->frame[pDoc->play_frame]->imageData[i*pDoc->frame[pDoc->play_frame]->widthStep+j*3+2],
						pDoc->frame[pDoc->play_frame]->imageData[i*pDoc->frame[pDoc->play_frame]->widthStep+j*3+1],
						pDoc->frame[pDoc->play_frame]->imageData[i*pDoc->frame[pDoc->play_frame]->widthStep+j*3])
						);
				}
			}
			CPen pen;
			pen.CreatePen(0,5,pDoc->m_pDrawDlg->m_color);
			memDC.SelectObject(pen);
			
			if (pDoc->state_editor==1){
				for(i=0;i<pDoc->cnt_point-1;i++){
					if (pDoc->m_point[i+1].x==-1){
						i++;
						continue;
					}
					memDC.MoveTo(pDoc->m_point[i].x,pDoc->m_point[i].y);
					memDC.LineTo(pDoc->m_point[i+1].x,pDoc->m_point[i+1].y);
				}
			}
			if (pDoc->check_save){
				for(i=0;i<pDoc->frame[pDoc->play_frame]->height;i++){
					for(j=0;j<pDoc->frame[pDoc->play_frame]->width;j++){

						f.val[0]=GetBValue(memDC.GetPixel(j,i));
						f.val[1]=GetGValue(memDC.GetPixel(j,i));
						f.val[2]=GetRValue(memDC.GetPixel(j,i));
						cvSet2D(pDoc->m_view,i,j,f);
					}
				}
				pDoc->check_save=false;
				cvNamedWindow("test");
				cvShowImage("test",pDoc->m_view);
				cvWaitKey();
				cvDestroyWindow("test");
	//			pDoc->cnt_point=0;
				cvCopy(pDoc->m_view,pDoc->frame[pDoc->play_frame]);
			}
			pen.DeleteObject();
		}

		pDC->BitBlt(0,0,rect.right, rect.bottom, &memDC, 0,0, SRCCOPY);//더블버퍼링
		memDC.DeleteDC();
	}
	else if(filetype.CompareNoCase(_T("BMP")) == 0)
	{
		CBitmapEx * pBitmap = new CBitmapEx();
		pBitmap ->Load(pDoc ->m_filename.GetBuffer());
		pBitmap ->Draw(this ->GetDC() ->m_hDC);
		delete pBitmap;
	}
	else if(filetype.CompareNoCase(_T("PPM")) == 0)
	{// PPM Processing
		int i,j;
		CDC memDC;
		CBitmap* OldBitmap,Bitmap;
		CRect rect;	
		this->GetClientRect(&rect);
		Bitmap.LoadBitmapW(pDoc ->m_filename);
		memDC.CreateCompatibleDC(pDC);
		OldBitmap=memDC.SelectObject(&Bitmap);
		//		memDC.PatBlt(0,0,rect.Width(),rect.Height(),WHITENESS);

		if (pDoc->cnt_frame>pDoc->play_frame){
			for(i=0;i<pDoc->frame[pDoc->play_frame]->height;i++){
				for(j=0;j<pDoc->frame[pDoc->play_frame]->width;j++){
				}
			}
		}


		pDC->BitBlt(0,0,rect.right, rect.bottom, &memDC, 0,0, SRCCOPY);//더블버퍼링
		memDC.DeleteDC();

		/*		ColorImage source;
		CString tmp = pDoc ->m_filename;
		char filename[1000];
		WideCharToMultiByte(CP_ACP,0,tmp,-1, filename,sizeof(filename),NULL,NULL);
		int width, height;
		sven::rgb *tmpRgb;
		tmpRgb = (sven::rgb *)source.load(filename, &width, & height);
		int i,j;
		CDC memDC;
		CBitmap* OldBitmap,Bitmap;
		CRect rect;	
		this->GetClientRect(&rect);
		Bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
		memDC.CreateCompatibleDC(pDC);
		OldBitmap=memDC.SelectObject(&Bitmap);
		memDC.PatBlt(0,0,rect.Width(),rect.Height(),WHITENESS);

		//sven::rgb * tmpRgb = new sven::rgb[width * height];
		for(i = 0 ; i < height; i++)
		{
		for( j = 0 ; j < width; j++)
		{
		// Get RGB Data. and display it on the view
		//				mpRgb[i * height + j] = source.data[i * height + j].to_rgb();
		COLORREF colorref = RGB(tmpRgb[i * height + j].r,
		tmpRgb[i * height + j].g, tmpRgb[i * height + j].b);
		memDC.SetPixel(j, i, colorref);
		}
		}

		pDC->BitBlt(0,0,rect.right, rect.bottom, &memDC, 0,0, SRCCOPY);//더블버퍼링
		memDC.DeleteDC();*/
	}
}


// CCartoonMakerView 인쇄

BOOL CCartoonMakerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CCartoonMakerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CCartoonMakerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CCartoonMakerView 진단

#ifdef _DEBUG
void CCartoonMakerView::AssertValid() const
{
	CView::AssertValid();
}

void CCartoonMakerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCartoonMakerDoc* CCartoonMakerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCartoonMakerDoc)));
	return (CCartoonMakerDoc*)m_pDocument;
}
#endif //_DEBUG


// CCartoonMakerView 메시지 처리기

void CCartoonMakerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar==13){
		CCartoonMakerDoc * pDoc = GetDocument();
		pDoc->play_frame++;
		pDoc->cnt_point=0;
		pDoc->m_pSliderDlg->m_sliderBus.SetPos(pDoc->play_frame);
		pDoc->m_pSliderDlg->m_strStatic.Format(_T("%d"),pDoc->play_frame);
		pDoc->m_pSliderDlg->UpdateData(false);
		Invalidate(false);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CCartoonMakerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	lpCreateStruct->cx=200;
	lpCreateStruct->cy=200;
	return 0;
}

BOOL CCartoonMakerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;
}

void CCartoonMakerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CString fileExtension = _T("");
	AfxGetMainWnd() ->GetWindowText(fileExtension);
	CMenu menu;
	CMenu *pSubMenu = NULL;
	CDocument * pDoc = GetDocument();
	ASSERT(pDoc);
	menu.LoadMenuW(IDR_EXTRAMENUS);
	fileExtension = fileExtension.Right(3);
	if(fileExtension.CompareNoCase(_T("AVI")) == 0)
		pSubMenu = menu.GetSubMenu(0);
	else if(fileExtension.CompareNoCase(_T("PPM")) == 0)
		pSubMenu = menu.GetSubMenu(1);
	else if(fileExtension.CompareNoCase(_T("BMP")) == 0)
	{
		pSubMenu = menu.GetSubMenu(2);
	}


	//	ClientToScreen(&point);
	if(pSubMenu == NULL)
		return;
	pSubMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CCartoonMakerView::OnAviLine()
{
	// TODO: Add your command handler code here
	CBitmapEx *pbitmap;
	pbitmap = new CBitmapEx();
	LPBYTE pBitmapdata = pbitmap ->GetData();
}

static BOOL bLButtonDown = FALSE;
CvPoint queue[1000000];
bool visit[1000][1000];
int head,tail;
void CCartoonMakerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	bLButtonDown = TRUE;
	int i,j;
	CCartoonMakerDoc *pDoc = GetDocument();
	if(pDoc ->m_filename.Right(3).CompareNoCase(_T("BMP")) == 0)
	{
		if(pDoc ->m_pDrawDlg ->m_bSetPixel == TRUE)
		{
			COLORREF color = GetDocument() ->m_pDrawDlg ->m_color;
			CBitmapEx *pBitmap = GetDocument()  ->m_pDrawDlg ->m_pStaticBitmap;
			pBitmap ->SetPixel(point.x, point.y, color);
			pBitmap ->SetPixel(point.x+1, point.y, color);
			pBitmap ->SetPixel(point.x-1, point.y, color);
			pBitmap ->SetPixel(point.x, point.y+1, color);
			pBitmap ->SetPixel(point.x, point.y-1, color);
			pBitmap ->SetPixel(point.x+1, point.y+1, color);
			pBitmap ->SetPixel(point.x-1, point.y-1, color);
			pBitmap ->SetPixel(point.x+1, point.y-1, color);
			pBitmap ->SetPixel(point.x-1, point.y+1, color);
			pBitmap ->SetPixel(point.x, point.y-2, color);
			pBitmap ->SetPixel(point.x, point.y+2, color);
			pBitmap ->SetPixel(point.x+2, point.y, color);
			pBitmap ->SetPixel(point.x-2, point.y, color);
			pBitmap ->Draw(this ->GetDC() ->m_hDC);
		}
	}
	else if(pDoc->state_editor==0){
		CvScalar f=cvGet2D(pDoc->frame[pDoc->play_frame],point.y,point.x);
		pDoc->m_pDrawDlg->m_color=RGB(f.val[2],f.val[1],f.val[0]);
	}
	else if(pDoc->state_editor==1){
		pDoc->m_point[pDoc->cnt_point++]=point;
		pDoc->m_point[pDoc->cnt_point++]=point;
		Invalidate(false);
	}
	else if(pDoc->state_editor==2){
		
		int wx[4]={-1,1,0,0};
		int wy[4]={0,0,1,-1};
		int CurrentX,CurrentY,NextX,NextY;
		for(i=0;i<pDoc->frame[pDoc->play_frame]->height;i++){
			for(j=0;j<pDoc->frame[pDoc->play_frame]->width;j++){
				visit[i][j]=0;
			}
		}
		head=0;
		tail=0;
		CvScalar StartColor=cvGet2D(pDoc->frame[pDoc->play_frame],point.y,point.x);
		CvScalar f,FillColor;
		FillColor.val[0]=GetBValue(pDoc->m_pDrawDlg->m_color);
		FillColor.val[1]=GetGValue(pDoc->m_pDrawDlg->m_color);
		FillColor.val[2]=GetRValue(pDoc->m_pDrawDlg->m_color);
		queue[0].x=point.x;
		queue[0].y=point.y;
		visit[point.y][point.x]=1;
		tail=1;
		do{

			CurrentX=queue[head].x;
			CurrentY=queue[head].y;

			cvSet2D(pDoc->frame[pDoc->play_frame],CurrentY,CurrentX,FillColor);
			head++;
			for(i=0;i<4;i++){
				NextX=CurrentX+wx[i];
				NextY=CurrentY+wy[i];
				if (0<=NextX && NextX<pDoc->frame[pDoc->play_frame]->width && 0<=NextY && NextY<pDoc->frame[pDoc->play_frame]->height && visit[NextY][NextX]==0){
					f=cvGet2D(pDoc->frame[pDoc->play_frame],NextY,NextX);
					if (f.val[0]==StartColor.val[0] && f.val[1]==StartColor.val[1] && f.val[2]==StartColor.val[2]){
						visit[NextY][NextX]=1;
						queue[tail].x=NextX;
						queue[tail].y=NextY;
						tail++;
					}
				}
			}
		}while(tail>head);
		Invalidate(false);
		
	}

	CView::OnLButtonDown(nFlags, point);
}

void CCartoonMakerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CCartoonMakerDoc *pDoc = GetDocument();
	bLButtonDown = FALSE;
	pDoc->m_point[pDoc->cnt_point++].x=-1;
	Invalidate(false);
	CView::OnLButtonUp(nFlags, point);
}

void CCartoonMakerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CCartoonMakerDoc *pDoc = GetDocument();
	if(pDoc ->m_filename.Right(3).CompareNoCase(_T("BMP")) == 0)
	{
		if(pDoc ->m_pDrawDlg ->m_bDrawLine == TRUE && bLButtonDown == TRUE)
		{
			COLORREF color = GetDocument() ->m_pDrawDlg ->m_color;
			CBitmapEx *pBitmap = GetDocument()  ->m_pDrawDlg ->m_pStaticBitmap;
			pBitmap ->SetPixel(point.x, point.y, color);
			pBitmap ->SetPixel(point.x+1, point.y, color);
			pBitmap ->SetPixel(point.x-1, point.y, color);
			pBitmap ->SetPixel(point.x, point.y+1, color);
			pBitmap ->SetPixel(point.x, point.y-1, color);
			pBitmap ->SetPixel(point.x+1, point.y+1, color);
			pBitmap ->SetPixel(point.x-1, point.y-1, color);
			pBitmap ->SetPixel(point.x+1, point.y-1, color);
			pBitmap ->SetPixel(point.x-1, point.y+1, color);
			pBitmap ->SetPixel(point.x, point.y-2, color);
			pBitmap ->SetPixel(point.x, point.y+2, color);
			pBitmap ->SetPixel(point.x+2, point.y, color);
			pBitmap ->SetPixel(point.x-2, point.y, color);
			pBitmap ->Draw(this ->GetDC() ->m_hDC);
		}
	}
	else if(nFlags&MK_LBUTTON){
		pDoc->m_point[pDoc->cnt_point++]=point;
		Invalidate(false);
	}


	CView::OnMouseMove(nFlags, point);
}

IplImage * ConvertImageToManga(IplImage *src){
	int i,j,y,x,k,sum,min,ind,v,r,g,b,CurrentX,CurrentY,NextX,NextY;
	int wx[4]={-1,1,0,0},wy[4]={0,0,1,-1};

	CvScalar avg;
	CvScalar CurrentColor;
	CvScalar NextColor;
	CvScalar Edge;
	double dev;

	IplImage * gray=cvCreateImage(cvSize(src->width,src->height),8,1);
	IplImage * dst=cvCreateImage(cvSize(src->width,src->height),8,1);
	IplImage * dst2=cvCreateImage(cvSize(src->width,src->height),8,3);
	cvCvtColor( src, gray, CV_BGR2GRAY );
	dst->origin=gray->origin;
	cvCanny(gray,dst,80,50);
	cvSmooth(src,dst2,CV_MEDIAN,3);
	cvCopyImage(dst2,src);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			visit[i][j]=0;				
		}
	}

	for(i=0;i<dst->imageSize;i++){
		dst->imageData[i]=255-dst->imageData[i];
	}

	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			avg=cvGet2D(dst,i,j);
			if (avg.val[0]<128){
				visit[i][j]=1;
				//			cvSet2D(dst2,i,j,cvScalar(0,0,0));
			}
		}
	}
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			if (visit[i][j]==0){
				head=0;
				tail=1;
				avg=cvGet2D(src,i,j);

				queue[0].y=i;
				queue[0].x=j;
				visit[i][j]=1;
				do{
					CurrentX=queue[head].x;
					CurrentY=queue[head].y;
					CurrentColor=cvGet2D(src,CurrentY,CurrentX);
					head++;
					for(k=0;k<4;k++){
						NextX=CurrentX+wx[k];
						NextY=CurrentY+wy[k];
						if (0<=NextX && NextX<src->width && 0<=NextY && NextY<src->height && visit[NextY][NextX]==0){
							NextColor=cvGet2D(src,NextY,NextX);

							dev=(NextColor.val[0]-CurrentColor.val[0])*(NextColor.val[0]-CurrentColor.val[0]);
							dev+=(NextColor.val[1]-CurrentColor.val[1])*(NextColor.val[1]-CurrentColor.val[1]);
							dev+=(NextColor.val[2]-CurrentColor.val[2])*(NextColor.val[2]-CurrentColor.val[2]);

							if (dev<10){
								queue[tail].x=NextX;
								queue[tail].y=NextY;

								avg.val[0]+=CurrentColor.val[0];
								avg.val[1]+=CurrentColor.val[1];
								avg.val[2]+=CurrentColor.val[2];

								visit[NextY][NextX]=1;
								tail++;
							}
						}
					}
				}while(head<tail);
				avg.val[0]/=tail;
				avg.val[1]/=tail;
				avg.val[2]/=tail;

				for(k=0;k<tail;k++){
					cvSet2D(dst2,queue[k].y,queue[k].x,avg);	
				}
			}
		}
	}

	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			avg=cvGet2D(dst2,i,j);

			avg.val[0]=avg.val[0]*1.5-60;
			avg.val[1]=avg.val[1]*1.5-60;
			avg.val[2]=avg.val[2]*1.5-60;

			cvSet2D(dst2,i,j,avg);
		}
	}

	cvReleaseImage(&dst);
	cvReleaseImage(&gray);
	return dst2;
}
UINT Cartoon(LPVOID value){
	int i,j;
	CCartoonMakerDoc * pDoc=(CCartoonMakerDoc *)value;
	for(i=0;i<pDoc->cnt_frame;i++){
		pDoc->frame[i]=ConvertImageToManga(pDoc->frame[i]);
	}
	return 0;
}
void CCartoonMakerView::OnAviCartoonmaking()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCartoonMakerDoc *pDoc = GetDocument();
	AfxBeginThread(Cartoon, (LPVOID) pDoc);	
	Invalidate();
}

void CCartoonMakerView::OnConvertOneFrameToManga()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCartoonMakerDoc *pDoc = GetDocument();
	pDoc->frame[pDoc->play_frame]=ConvertImageToManga(pDoc->frame[pDoc->play_frame]);
	Invalidate();
}

void CCartoonMakerView::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

