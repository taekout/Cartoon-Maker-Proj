// CartoonMakerDoc.cpp : CCartoonMakerDoc 클래스의 구현
//

#include "stdafx.h"
#include "CartoonMaker.h"
#include "BitmapEx.h"
#include "CartoonMakerDoc.h"
#include "ChildFrm.h"
#include "images.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define GRID_WIDTH       640
//#define GRID_HEIGHT      350
//#define CARTOON_WIDTH    1300
//#define CARTOON_HEIGHT   1300

#define CARTOON_FRAME_NUMBER 2000


#define BACKGROUND_WHITE (((255) << 16) | ((255) << 8) | (255))
#define BACKGROUND_BLACK (((0) << 16) | ((0) << 8) | (0))
#define BACKGROUND_RED (((255) << 16) | ((0) << 8) | (0))
#define BACKGROUND_GREEN (((0) << 16) | ((255) << 8) | (0))
#define BACKGROUND_BLUE (((0) << 16) | ((0) << 8) | (255))


CFrameWnd * g_pointer = NULL;

// CCartoonMakerDoc

IMPLEMENT_DYNCREATE(CCartoonMakerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCartoonMakerDoc, CDocument)
	ON_COMMAND(ID_FILE_CAM, &CCartoonMakerDoc::OnFileCam)
	ON_COMMAND(REC, &CCartoonMakerDoc::OnRec)
	ON_COMMAND(STOP, &CCartoonMakerDoc::OnStop)
	ON_COMMAND(ID_PPM_COLOR2GREY, &CCartoonMakerDoc::OnPpmColor2grey)
	ON_COMMAND(ID_BLUR, &CCartoonMakerDoc::OnBlur)
	ON_COMMAND(ID_LUMINANCE_BRIGHTER, &CCartoonMakerDoc::OnLuminanceBrighter)
	ON_COMMAND(ID_LUMINANCE_DARKER, &CCartoonMakerDoc::OnLuminanceDarker)
	ON_COMMAND(ID_EFFECTS_FIRE, &CCartoonMakerDoc::OnEffectsFire)
	ON_COMMAND(ID_EFFECTS_WATER, &CCartoonMakerDoc::OnEffectsWater)
	ON_COMMAND(ID_AVI_HATCHING, &CCartoonMakerDoc::OnAviHatching)
	ON_COMMAND(ID_FILE_SAVE_AS, &CCartoonMakerDoc::OnFileSaveAs)
	ON_COMMAND(ID_AVI_CHOOSEFRAME, &CCartoonMakerDoc::OnAviChooseframe)
	ON_COMMAND(ID_AVI_CARTOONMAKINGWITHCHOSENFRAMES, &CCartoonMakerDoc::OnAviCartoonmakingwithchosenframes)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_RED, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonRed)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_GREEN, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonGreen)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_BLUE, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonBlue)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_BLACK, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonBlack)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_WHITE, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonWhite)
	ON_COMMAND(ID_SETBACKGROUNDCOLORFORCARTOON_USERDEFINEDCOLOR, &CCartoonMakerDoc::OnSetbackgroundcolorforcartoonUserdefinedcolor)
END_MESSAGE_MAP()


// CCartoonMakerDoc 생성/소멸
CString g_filename;

// Cartoon background color
DWORD g_CartoonBackgroundColor = 0;

CCartoonMakerDoc::CCartoonMakerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	capture=NULL;
	cnt_frame=0;
	check_rec=0;
	check_save=false;
	play_frame=0;
	cnt_point=0;
	m_view=NULL;
	m_pDrawDlg = NULL;
	m_pSliderDlg = NULL;
	state_editor=0;

//	m_bitmapUsedForCartoonMaking.Clear(BACKGROUND_WHITE);
}

CCartoonMakerDoc::~CCartoonMakerDoc()
{
	int i;
	cvReleaseCapture( &capture );
	for(i=0;i<cnt_frame;i++){
		cvReleaseImage(&frame[i]);
	}
	if (m_view)cvReleaseImage(&m_view);
}

// CCartoonMakerDoc 명령
UINT CamView(LPVOID value){
	IplImage * frame_copy;
	CCartoonMakerDoc * pDoc=(CCartoonMakerDoc *)value;
	pDoc->capture = cvCaptureFromCAM(0);
	if( pDoc->capture )
	{

		for(pDoc->cnt_frame=0;pDoc->cnt_frame<CARTOON_FRAME_NUMBER;pDoc->cnt_frame+=pDoc->check_rec)
		{
			if( !cvGrabFrame( pDoc->capture ))
				break;
			frame_copy=cvRetrieveFrame( pDoc->capture );
			pDoc->frame[pDoc->cnt_frame]=cvCreateImage(cvSize(frame_copy->width,frame_copy->height),8,3);
			cvCopy(frame_copy,pDoc->frame[pDoc->cnt_frame]);
			cvShowImage("cam",frame_copy);
			cvWaitKey(1);

			if(g_pointer != NULL)
			{
				LPARAM windowWidth = frame_copy ->width;
				WPARAM windowHeight = frame_copy ->height;
				HWND handleWnd = g_pointer ->GetSafeHwnd();

				BOOL reB= ::PostMessage(handleWnd
					, WM_WINDOW_SIZE_FOR_PICTURE
					, windowWidth
					, windowHeight);
			}


		}
	}

	cvReleaseImage(&frame_copy);
	return 0;
}

BOOL CCartoonMakerDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	cvNamedWindow("Cam");
	g_pointer = ((CMDIFrameWnd *)::AfxGetMainWnd()) ->GetActiveFrame();
	AfxBeginThread(CamView, (LPVOID) this);

	if(m_pDrawDlg == NULL)
	{
		m_pDrawDlg = new CDrawDlg();
		this ->m_pDrawDlg ->Create(IDD_FINETWEAKDLG);
		this ->m_pDrawDlg ->ShowWindow(SW_SHOW);
	}

	if(m_pSliderDlg == NULL)
	{
		m_pSliderDlg = new CSliderDlg();
		this ->m_pSliderDlg ->Create(IDD_SLIDERDLG);
		this ->m_pSliderDlg ->MoveWindow(0, GetSystemMetrics(SM_CYFULLSCREEN) - 100
			,GetSystemMetrics(SM_CXFULLSCREEN) , 100);
		this ->m_pSliderDlg ->ShowWindow(SW_SHOW);
	}
	return TRUE;
}



// CCartoonMakerDoc serialization
UINT Loading(LPVOID value){
	int i;
	IplImage * frame_copy;
	CCartoonMakerDoc * pDoc=(CCartoonMakerDoc *)value;
	//	Set the size of an image.
	//	CRect size(0, frame_copy->width, 0, frame_copy->height);



	if( pDoc->capture )
	{
		for(pDoc->cnt_frame=0;pDoc->cnt_frame<CARTOON_FRAME_NUMBER;pDoc->cnt_frame++)
		{

			if( !cvGrabFrame( pDoc->capture ))
				break;
			frame_copy=cvRetrieveFrame( pDoc->capture );
			pDoc->frame[pDoc->cnt_frame]=cvCreateImage(cvSize(frame_copy->width,frame_copy->height),8,3);
			//			cvConvertImage(frame_copy,pDoc->frame[pDoc->cnt_frame],CV_CVTIMG_FLIP);
			cvConvertImage(frame_copy,pDoc->frame[pDoc->cnt_frame],CV_CVTIMG_FLIP);		
			if (pDoc->m_view==0){
					pDoc->m_view=cvCreateImage(cvGetSize(pDoc->frame[0]),pDoc->frame[0]->depth,pDoc->frame[0]->nChannels);
			}

			// Send a message that contains the image size so that the window frame will adjust

			// This code cannot work because the usual width is bigger than 255(the maximum value)
			//			LPARAM windowSize = (frame_copy ->width << 8) | (frame_copy ->height);

			// So I chose a different way

			LPARAM windowWidth = frame_copy ->width;
			WPARAM windowHeight = frame_copy ->height;

			if(g_pointer != NULL)
			{
				HWND handleWnd = g_pointer ->GetSafeHwnd();

				BOOL reB= ::PostMessage(handleWnd
					, WM_WINDOW_SIZE_FOR_PICTURE
					, windowWidth
					, windowHeight);
				INT result = GetLastError();
				CString str;
				str.Format(_T("%d"), result);
				//				AfxMessageBox(str);
			}

		}

	}
	//	cvReleaseImage(&frame_copy);
	return 0;
}

void CCartoonMakerDoc::Serialize(CArchive& ar)
{
	char Buffer[255];
	int i;

	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		WideCharToMultiByte(CP_ACP,0,ar.m_strFileName,-1,Buffer,sizeof(Buffer),NULL,NULL);
		capture=cvCaptureFromFile(Buffer);
		g_pointer = ((CMDIFrameWnd *)::AfxGetMainWnd()) ->GetActiveFrame();
		AfxBeginThread(Loading, (LPVOID) this);

		CMenu *pMenu = (CMenu *)NULL , *pAVIMenu = (CMenu *)NULL;
		pMenu= new CMenu();
		pMenu ->LoadMenuW(IDR_EXTRAMENUS);
		pAVIMenu = pMenu ->GetSubMenu(0);
		CMenu *pMainMenu = AfxGetMainWnd() ->GetMenu();
		pMainMenu ->AppendMenuW(MF_STRING, (UINT_PTR)pAVIMenu);
		SetMenu(AfxGetMainWnd() ->GetSafeHwnd(), pMainMenu ->m_hMenu);

		//		::AfxGetMainWnd() ->GetMenu() ->AppendMenuW(MF_STRING, IDR_AVIMENU);
	}
}


// CCartoonMakerDoc 진단

#ifdef _DEBUG
void CCartoonMakerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCartoonMakerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CCartoonMakerDoc::OnFileCam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cvNamedWindow("Cam");
	g_pointer = ((CMDIFrameWnd *)::AfxGetMainWnd()) ->GetActiveFrame();
	AfxBeginThread(CamView, (LPVOID) this);

	if(m_pDrawDlg == NULL)
	{
		m_pDrawDlg = new CDrawDlg();
		this ->m_pDrawDlg ->Create(IDD_FINETWEAKDLG);
		this ->m_pDrawDlg ->ShowWindow(SW_SHOW);
	}

	if(m_pSliderDlg == NULL)
	{
		m_pSliderDlg = new CSliderDlg();
		this ->m_pSliderDlg ->Create(IDD_SLIDERDLG);
		this ->m_pSliderDlg ->MoveWindow(0, GetSystemMetrics(SM_CYFULLSCREEN) - 100
			,GetSystemMetrics(SM_CXFULLSCREEN) , 100);
		this ->m_pSliderDlg ->ShowWindow(SW_SHOW);
	}
}

void CCartoonMakerDoc::OnRec()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	check_rec=1;
}

void CCartoonMakerDoc::OnStop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	check_rec=0;
}

BOOL CCartoonMakerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CString bmpFilename = lpszPathName;
	// TODO:  Add your specialized creation code here
	if(bmpFilename.Right(3).CompareNoCase(_T("PNG")) == 0)
	{
		IplImage *pImage = this ->Hatching(bmpFilename);
		return TRUE;
	}



	if(m_pDrawDlg == NULL)
	{
		m_pDrawDlg = new CDrawDlg();
		this ->m_pDrawDlg ->Create(IDD_FINETWEAKDLG);
		this ->m_pDrawDlg ->ShowWindow(SW_SHOW);
		this ->m_pDrawDlg ->m_pDoc=(LPVOID * )this;
		this ->m_pDrawDlg ->m_filename = bmpFilename;
		this ->m_pDrawDlg ->SetMainFramePointer((CMainFrame *)AfxGetMainWnd());
	}

	if(m_pSliderDlg == NULL)
	{
		m_pSliderDlg = new CSliderDlg();
		this ->m_pSliderDlg ->Create(IDD_SLIDERDLG);
		this ->m_pSliderDlg ->MoveWindow(0, GetSystemMetrics(SM_CYFULLSCREEN) - 100
			,GetSystemMetrics(SM_CXFULLSCREEN) , 100);
		this ->m_pSliderDlg ->ShowWindow(SW_SHOW);
		this ->m_pSliderDlg ->m_pDoc = this;
	}

	// In case of BMP or other graphics file types, 
	// post a window-sizing message for the corresponding childframe.

	if(bmpFilename.Right(3).CompareNoCase(_T("BMP")) == 0)
	{
		CBitmapEx *pBitmap  = new CBitmapEx();
		pBitmap ->Load((LPTSTR)lpszPathName);
		// 만약 해당 비트맵 파일이 존재치 않는다면 동적 에러가 발생할 것입니다.

		BOOL reB= ::PostMessage(AfxGetMainWnd() ->GetSafeHwnd()
			, WM_WINDOW_SIZE_FOR_PICTURE
			, pBitmap ->GetWidth()
			, pBitmap ->GetHeight());
		this ->m_pDrawDlg ->m_pStaticBitmap = pBitmap;;
	}


	CString tmp = lpszPathName;
	INT curPos = 0, previousPos;
	while(curPos < tmp.GetLength())
	{
		previousPos = curPos ;
		curPos = tmp.Find(_T("\\"), curPos);
		if(curPos == -1)
			break;
		curPos ++;
	}

	this ->m_filename = tmp.Mid(previousPos, tmp.GetLength() - previousPos);

	// XML Parsing and if there is a duplicate, no need to add an element.
	// If the same file doesn't exist in the Xml database, there is the need to add an element.
	CXmlParser parser;
	CString xmlDoc = parser.LoadXmlFile(FILE_DATABASE_NAME);

	if(xmlDoc.CompareNoCase(_T("unable to open file")) == 0)
	{
		xmlDoc = "";
		// if the file doesn't exist.. create the basic structure.
		parser.CreateFileManager();
		//		AfxMessageBox(_T("A new file will be created"));
	}

	//	Xml Parsing and data process
	parser.SetDoc(xmlDoc);
	CString filename = _T("");
	CString filetype = _T("");
	BOOL bExisted = FALSE;
	while (parser.FindChildElem(_T("Video")))
	{
		filename = parser.GetChildAttrib(_T("videoFileName"));
		filetype = parser.GetChildAttrib(_T("extension"));
		if(filename.Compare(lpszPathName) == 0)
		{	// if the file already exists in the database, no need for saving.
			bExisted = TRUE;
		}
	}
	if(bExisted == TRUE)
	{	// if the opened file already exists in DB
		//Do nothing.
	}
	else
	{	// it doesn't exist!
		// Gotta add the element
		// from the root node
		BOOL res = FALSE;
		parser.ResetPos();
		res = parser.FindElem();	// the root element.
		parser.IntoElem();
		// Here it tries to add the new element.
		res = parser.AddElem(_T("Video"));
		res = parser.SetAttrib(_T("videoFileName"), lpszPathName);
		CString tmp = lpszPathName;
		parser.SetAttrib(_T("extension"), tmp.Right(3));
		parser.SetAttrib(_T("title"), lpszPathName);
		parser.IntoElem();
		parser.AddElem(_T("genre"), _T("drama"));
		parser.AddElem(_T("rating"), _T("4"));
		parser.AddElem(_T("user_rating"), _T("5"));
		parser.AddElem(_T("summary"), _T("This is totally a blast!"));
		parser.AddElem(_T("details"), _T("The setting : The medieval"));
		parser.AddElem(_T("year"), _T("1993"));
		parser.AddElem(_T("director"), _T("Jack Nicolson"));
		parser.AddElem(_T("studio"), _T("Paramount"));
		parser.AddElem(_T("runtime"), _T("128"));
		parser.OutOfElem();

		CFile file(FILE_DATABASE_NAME, CFile::modeCreate | CFile::modeWrite);
		CString wholeDoc = parser.GetDoc();
		char  * pBuf = new char[wholeDoc.GetLength() + 5];
		for(INT i = 0 ; i < wholeDoc.GetLength() + 5 ; i++)
		{
			pBuf[i] = 0;
		}
		::WideCharToMultiByte(CP_ACP, 0, wholeDoc, -1
			, pBuf, wholeDoc.GetLength() + 1, NULL,NULL);
		INT len = strlen(pBuf);
		file.Write(pBuf, wholeDoc.GetLength());
		file.Close();
		parser.ResetPos();
	}


	return TRUE;
}

// CCartoonMakerDoc 명령
UINT Color2Grey_Thread(LPVOID value){
	ColorImage::MainOfColor2grey(g_filename);
	return	1;
}

void CCartoonMakerDoc::OnPpmColor2grey()
{
	// TODO: Add your command handler code here
	g_filename = m_filename;
	AfxBeginThread(Color2Grey_Thread, (LPVOID) this);
}

void CCartoonMakerDoc::OnBlur()
{
	// TODO: Add your command handler code here
	CBitmapEx * pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());

	pBitmap ->GaussianBlur();

	POSITION pos = this ->GetFirstViewPosition();
	CView *pView = this ->GetNextView(pos);
	pBitmap ->Draw(pView ->GetDC() ->m_hDC);
}

static INT nBrightness = 0;
void CCartoonMakerDoc::OnLuminanceBrighter()
{
	// TODO: Add your command handler code here

	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	nBrightness += 20;
	pBitmap ->Brightness(nBrightness);

	POSITION pos = this ->GetFirstViewPosition();
	CView *pView = this ->GetNextView(pos);
	pBitmap ->Draw(pView ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CCartoonMakerDoc::OnLuminanceDarker()
{
	// TODO: Add your command handler code here

	CBitmapEx *pBitmap = NULL;
	pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	nBrightness -= 20 ;
	pBitmap ->Brightness(nBrightness);

	POSITION pos = this ->GetFirstViewPosition();
	CView *pView = this ->GetNextView(pos);
	pBitmap ->Draw(pView ->GetDC() ->m_hDC);
	delete pBitmap;
}

void CCartoonMakerDoc::OnEffectsFire()
{
	// TODO: Add your command handler code here
	CBitmapEx *pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	pBitmap ->CreateFireEffect();
	POSITION pos = GetFirstViewPosition();
	CView *pView = GetNextView(pos);

	for(INT i = 0 ; i < 30; i++)
	{
		pBitmap ->UpdateFireEffect();
		pBitmap ->Draw(pView ->GetDC() ->m_hDC);
	}
	delete pBitmap;
}

void CCartoonMakerDoc::OnEffectsWater()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CBitmapEx *pBitmap = new CBitmapEx();
	pBitmap ->Load(m_filename.GetBuffer());
	pBitmap ->CreateWaterEffect();

	pBitmap ->MakeWaterBlob(0,0, 20, 20);
	pBitmap ->MakeWaterBlob(100,100, 30, 10);
	pBitmap ->MakeWaterBlob(120,180, 20, 20);
	pBitmap ->MakeWaterBlob(140,30, 20, 20);
	pBitmap ->MakeWaterBlob(200,280, 20, 20);
	pBitmap ->MakeWaterBlob(310,40, 20, 20);
	pBitmap ->MakeWaterBlob(180,300, 20, 20);
	pBitmap ->MakeWaterBlob(80,180, 20, 20);
	pBitmap ->MakeWaterBlob(420,280, 200, 20);
	POSITION pos = GetFirstViewPosition();
	CView *pView = GetNextView(pos);
	for(INT i = 0 ; i < 50 ; i++)
	{
		pBitmap ->UpdateWaterEffect();
		pBitmap ->Draw(pView ->GetDC() ->m_hDC);
	}
	delete pBitmap;
}

unsigned char g_Tmp3DimensionalArr[100][5][5];
IplImage * Hatching(IplImage * src){
	int i,j,y,x,k,sum,min,ind,v;
	unsigned img[5][5];
	CvScalar f;
	IplImage * gray=cvCreateImage(cvSize(src->width,src->height),8,1);
	IplImage * dst=cvCreateImage(cvSize(src->width,src->height),8,1);
	cvCvtColor( src, gray, CV_BGR2GRAY );

	dst->origin=gray->origin;
	cvCanny(gray,dst,160,120);
	for(i=0;i<dst->imageSize;i++)dst->imageData[i]=255-dst->imageData[i];

	for(i=0;i<dst->height-10;i+=5){
		for(j=0;j<dst->width-10;j+=5){
			sum=0;
			for(y=0;y<5;y++){
				for(x=0;x<5;x++){
					img[y][x]=cvGet2D(gray,i+y,j+x).val[0];
					sum+=cvGet2D(dst,i+y,j+x).val[0];
				}
			}
			if (sum>6000){
				min=9999999;
				for(k=0;k<31;k++){
					sum=0;
					for(y=0;y<5;y++){
						for(x=0;x<5;x++){
							v=img[y][x]-g_Tmp3DimensionalArr[k][y][x];
							if (v<0)v=-v;
							sum+=v;
						}
					}
					if (sum<min){
						min=sum;
						ind=k;
					}
				}
				for(y=0;y<5;y++){
					for(x=0;x<5;x++){
						f.val[0]=g_Tmp3DimensionalArr[ind][y][x];
						cvSet2D(dst,y+i,x+j,f);
					}
				}
			}
		}
	}
	cvReleaseImage(&gray);
	return dst;
}

/*void CCartoonMakerDoc::OnPngHatching()
{
// TODO: Add your command handler code here
int i,j,k;
IplImage * pt=cvLoadImage("1.bmp");

for(k=0;k<31;k++){
for(i=0;i<5;i++){
for(j=0;j<5;j++){
g_Tmp3DimensionalArr[k][i][j]=cvGet2D(pt,i+k*5,j).val[0];
}
}
}


IplImage * src=cvLoadImage("TEST.png");
if(src == NULL)
AfxMessageBox(_T("The file was not found."));
cvNamedWindow("result");
cvShowImage("result",Hatching(src));
cvWaitKey();
cvDestroyWindow("result");
cvReleaseImage(&src);
cvReleaseImage(&pt);
return;
}


void CCartoonMakerDoc::OnPngHatching()
{
// TODO: Add your command handler code here
// TODO: Add your command handler code here
int i,j,k;
IplImage * pt=cvLoadImage("1.bmp");

for(k=0;k<31;k++){
for(i=0;i<5;i++){
for(j=0;j<5;j++){
g_Tmp3DimensionalArr[k][i][j]=cvGet2D(pt,i+k*5,j).val[0];
}
}
}


IplImage * src=cvLoadImage("TEST.png");
if(src == NULL)
AfxMessageBox(_T("The file was not found."));
cvNamedWindow("result");
cvShowImage("result",Hatching(src));
cvWaitKey();
cvDestroyWindow("result");
cvReleaseImage(&src);
cvReleaseImage(&pt);
return;
}

*/
IplImage *CCartoonMakerDoc::Hatching(CString filename)
{
	int i,j,k;
	char buffer[CARTOON_FRAME_NUMBER] = {0};
	IplImage * pt=cvLoadImage("1.bmp");

	for(k=0;k<31;k++){
		for(i=0;i<5;i++){
			for(j=0;j<5;j++){
				g_Tmp3DimensionalArr[k][i][j]=cvGet2D(pt,i+k*5,j).val[0];
			}
		}
	}

	WideCharToMultiByte(CP_ACP,0,filename.GetBuffer(),-1,buffer, sizeof(buffer), NULL, NULL);
	IplImage * src=cvLoadImage(buffer);
	if(src == NULL)
		AfxMessageBox(_T("The file was not found."));
	cvNamedWindow(buffer);
	cvShowImage(buffer,::Hatching(src));
	cvWaitKey();
	cvDestroyWindow("result");
	cvReleaseImage(&src);
	cvReleaseImage(&pt);
	return pt;
}

void CCartoonMakerDoc::OnAviHatching()
{
	// TODO: Add your command handler code here
	int i,j,k;
	char buffer[CARTOON_FRAME_NUMBER] = {0};
	IplImage * pt=cvLoadImage("1.bmp");

	for(k=0;k<31;k++){
		for(i=0;i<5;i++){
			for(j=0;j<5;j++){
				g_Tmp3DimensionalArr[k][i][j]=cvGet2D(pt,i+k*5,j).val[0];
			}
		}
	}

	WideCharToMultiByte(CP_ACP,0,m_filename.GetBuffer(),-1,buffer, sizeof(buffer), NULL, NULL);
	IplImage * src = frame[this ->play_frame];
	//	src ->dataOrder
	if(src == NULL)
		AfxMessageBox(_T("The file was not found."));
	cvNamedWindow(buffer);
	cvShowImage(buffer,::Hatching(src));
	cvWaitKey();
	//	cvDestroyWindow("result");
	//	cvReleaseImage(&src);
	//	cvReleaseImage(&pt);
	return;
}

void CCartoonMakerDoc::OnFileSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i;
	char Buffer[255];
	CFileDialog dlg(false);
	if (dlg.DoModal()==IDOK){
		WideCharToMultiByte(CP_ACP,0,dlg.GetPathName(),-1,Buffer,sizeof(Buffer),NULL,NULL);
		CvVideoWriter * vout = cvCreateVideoWriter(Buffer,5,30,cvSize(frame[0]->width,frame[0]->height),1);
		for(i=0;i<cnt_frame;i++){
			cvWriteFrame(vout,frame[i]);
		}
		cvReleaseVideoWriter(&vout);		
	}
}

void CCartoonMakerDoc::OnAviChooseframe()
{
	// TODO: Add your command handler code here
	m_listFrames.listChosenFrames.Add(this ->play_frame);
	m_listFrames.numberOfChosenFrames ++;
}

void CCartoonMakerDoc::OnAviCartoonmakingwithchosenframes()
{
	// TODO: Add your command handler code here

	CBitmapEx bitmap;
	bitmap.Create(frame[play_frame] ->width, frame[play_frame] ->height);
	LONG x = 0, y = 0 ;
	INT numberOfGrid = (m_listFrames.numberOfChosenFrames < 6) ? m_listFrames.numberOfChosenFrames : 6;
	
	int CARTOON_WIDTH=frame[0]->width*2+30,CARTOON_HEIGHT=frame[0]->height*3+40;

	int GRID_WIDTH=frame[0]->width,GRID_HEIGHT=frame[0]->height;

	m_bitmapUsedForCartoonMaking.Create(CARTOON_WIDTH, CARTOON_HEIGHT);

	for(INT i = 0 ; i <  numberOfGrid ; i++)
	{
		switch(i)
		{
		case 0 :
			//the first grid
			x = 10;
			y = 10;
			break;
		case 1 :
			x = 20+GRID_WIDTH;
			y = 10;
			break;
		case 2 :
			x = 10;
			y = 20+GRID_HEIGHT;
			break;
		case 3 :
			x = 20+GRID_WIDTH;
			y = 20+GRID_HEIGHT;
			break;
		case 4 :
			x = 10;
			y = 30+GRID_HEIGHT*2;
			break;
		case 5 :
			x = 20+GRID_WIDTH;
			y = 30+GRID_HEIGHT*2;
			break;
		default:
			break;
		}

		// Now X, Y coordinate is set.
		// Draw the image
		// at first , transfer iplimage data into a bitmap instance.
//		char *imageDataFromIplImage = this ->frame[play_frame]->imageData;
		LONG height = frame[play_frame] ->height;
		LONG width = frame[play_frame] ->width;
		for(INT tmp1 = 0 ; tmp1 < height ; tmp1++)
		{
			for(INT tmp2 = 0 ; tmp2 < width ; tmp2++)
			{
				bitmap.SetPixel(tmp2, tmp1, RGB(
					frame[m_listFrames.listChosenFrames.GetAt(i)]->imageData[tmp1*frame[m_listFrames.listChosenFrames.GetAt(i)]->widthStep+tmp2*3],
					frame[m_listFrames.listChosenFrames.GetAt(i)]->imageData[tmp1*frame[m_listFrames.listChosenFrames.GetAt(i)]->widthStep+tmp2*3+1],
					frame[m_listFrames.listChosenFrames.GetAt(i)]->imageData[tmp1*frame[m_listFrames.listChosenFrames.GetAt(i)]->widthStep+tmp2*3+2]));
			}
		}
		// Get a Bitmap from this ->frame[play_frame]... ==> Already got it!
		// then I can draw the image on the Bitmap file. (Make sure to stretch an image!)
//		HANDLE handle =   ::CopyImage(bitmap.GetData(), IMAGE_BITMAP, x, y, LR_COPYDELETEORG);
//		bitmap.SetData((LPBYTE)handle);

		// After transferring the stretched images, I can save it to a bitmap FILE.
		//m_bitmapUsedForCartoonMaking.Save(_T("Output.bmp"));
		m_bitmapUsedForCartoonMaking.Draw(x, y, GRID_WIDTH, GRID_HEIGHT, bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
		m_bitmapUsedForCartoonMaking.Save(_T("Output.bmp"));

		// Don't save it. save it in a bitmap class first and

	}

}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonRed()
{
	// TODO: Add your command handler code here
	g_CartoonBackgroundColor = BACKGROUND_RED;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonGreen()
{
	// TODO: Add your command handler code here
	g_CartoonBackgroundColor = BACKGROUND_GREEN;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonBlue()
{
	// TODO: Add your command handler code here
	g_CartoonBackgroundColor = BACKGROUND_BLUE;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonBlack()
{
	// TODO: Add your command handler code here
	g_CartoonBackgroundColor = BACKGROUND_BLACK;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonWhite()
{
	// TODO: Add your command handler code here
	g_CartoonBackgroundColor = BACKGROUND_WHITE;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}

void CCartoonMakerDoc::OnSetbackgroundcolorforcartoonUserdefinedcolor()
{
	// TODO: Add your command handler code here
	CColorDialog dlg(g_CartoonBackgroundColor);
	dlg.DoModal();
	g_CartoonBackgroundColor = dlg.GetColor();
	BYTE highest = g_CartoonBackgroundColor && ((255) << 16);
	BYTE median = g_CartoonBackgroundColor && ((255) << 8);
	BYTE lowest = g_CartoonBackgroundColor && (255);
	g_CartoonBackgroundColor = 0;
	g_CartoonBackgroundColor |= ( (highest) >> 16);
	g_CartoonBackgroundColor |= ( (lowest) << 16);
	g_CartoonBackgroundColor |= median;
	m_bitmapUsedForCartoonMaking.Clear(g_CartoonBackgroundColor);
}
