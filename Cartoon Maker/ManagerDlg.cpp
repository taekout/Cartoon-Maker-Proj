// ManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CartoonMaker.h"
#include "ManagerDlg.h"
#include "MainFrm.h"
#include "CartoonMakerDoc.h"


// CManagerDlg dialog

IMPLEMENT_DYNAMIC(CManagerDlg, CDialog)

CManagerDlg::CManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManagerDlg::IDD, pParent)
{
	m_managerXml.LoadXmlFile(FILE_DATABASE_NAME);
}

CManagerDlg::~CManagerDlg()
{
}

void CManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_FILELIST, m_fileList);
}


BEGIN_MESSAGE_MAP(CManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CManagerDlg::OnBnClickedTest)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILELIST, &CManagerDlg::OnLvnItemchangedFilelist)
ON_BN_CLICKED(IDC_DISPLAY, &CManagerDlg::OnBnClickedDisplay)
ON_BN_CLICKED(IDC_REMOVE, &CManagerDlg::OnBnClickedRemove)
END_MESSAGE_MAP()


// CManagerDlg message handlers

void CManagerDlg::OnBnClickedTest()
{
	// TODO: Add your control notification handler code here
	CString a = _T("JWG.Heroes.S03E07.HDTV.XviD-LOL.srt");
	CXmlParser xmlp;
	xmlp.ReadSub(a.GetBuffer());

	//tmp = pa.GetAttrib(_T("description"));
}

void CManagerDlg::OnLvnItemchangedFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

BOOL CManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_fileList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EX_GRIDLINES, CRect(10, 10, 300, 300), this, 200);

	m_fileList.SetExtendedStyle(m_fileList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_fileList.InsertColumn(0,_T("No"),LVCFMT_LEFT,50);
	m_fileList.InsertColumn(1,_T("File Name"),LVCFMT_LEFT,150);
	m_fileList.InsertColumn(2, _T("File Type"), LVCFMT_LEFT,50);

	m_managerXml.LoadXmlFile(FILE_DATABASE_NAME);
/*	CString xmlDoc = m_managerXml.GetDoc();
	m_managerXml.SetDoc(xmlDoc);
	m_managerXml.FindElem();
	m_managerXml.IntoElem();
	while(m_managerXml.FindElem(_T("Video")))
	{
		CString filename = m_managerXml.GetAttrib(_T("videoFileName"));
		CString filetype = m_managerXml.GetAttrib(_T("extension"));
		m_managerXml.OutOfElem();
		// Extract data you want.
//		m_managerXml.OutOfElem();		

		// I have to add the elements to the list ctrl.
		CString resToken = _T(""), previousToken = _T("");
		INT curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = filename.Tokenize(_T("\\"), curPos);
		}
		INT i = m_fileList.GetItemCount() + 1;
		CString str; str.Format(_T("%d"), i);
		m_fileList.InsertItem(m_fileList.GetItemCount(), str);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 1, previousToken);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 2, previousToken.Right(3));

	}*/

/*
	while(m_managerXml.FindChildElem(_T("Video")))
	{
		CString filename = m_managerXml.GetAttrib(_T("videoFileName"));
		CString filetype = m_managerXml.GetAttrib(_T("extension"));
//..	m_managerXml.OutOfElem();
		// Extract data you want.
//		m_managerXml.OutOfElem();		

		// I have to add the elements to the list ctrl.
		CString resToken = _T(""), previousToken = _T("");
		INT curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = filename.Tokenize(_T("\\"), curPos);
		}
		INT i = m_fileList.GetItemCount() + 1;
		CString str; str.Format(_T("%d"), i);
		m_fileList.InsertItem(m_fileList.GetItemCount(), str);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 1, previousToken);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 2, previousToken.Right(3));
	}*/

BOOL bFinished = FALSE;
m_managerXml.ResetPos();
CString doc = m_managerXml.GetDoc();
if ( ! m_managerXml.FindChildElem() )
    bFinished = TRUE;
while ( ! bFinished )
{
    // Process element

    m_managerXml.IntoElem();
    CString csTag = m_managerXml.GetTagName();
	if(csTag.Compare(_T("Video")) == 0)
	{
		CString filename = m_managerXml.GetAttrib(_T("videoFileName"));
		CString filetype = m_managerXml.GetAttrib(_T("extension"));
//..	m_managerXml.OutOfElem();
		// Extract data you want.
//		m_managerXml.OutOfElem();		

		// I have to add the elements to the list ctrl.
		CString resToken = _T(""), previousToken = _T("");
		INT curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = filename.Tokenize(_T("\\"), curPos);
		}
		INT i = m_fileList.GetItemCount() + 1;
		CString str; str.Format(_T("%d"), i);
		m_fileList.InsertItem(m_fileList.GetItemCount(), str);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 1, previousToken);
		m_fileList.SetItemText(m_fileList.GetItemCount() - 1, 2, previousToken.Right(3));
	}

    // Next element (depth first)

    BOOL bFound = m_managerXml.FindChildElem();
    while ( ! bFound && ! bFinished )
    {
        if ( m_managerXml.OutOfElem() )
            bFound = m_managerXml.FindChildElem();
        else
            bFinished = TRUE;
    }
}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CManagerDlg::OnBnClickedDisplay()
{
	// TODO: Add your control notification handler code here
 	POSITION pos = m_fileList.GetFirstSelectedItemPosition();
	UINT nColumn = m_fileList.GetNextSelectedItem(pos);
	CString filename= m_fileList.GetItemText(nColumn, 1);
	MessageBox(filename + _T(" will be opened"));

	// Here by any means necessary I will have to invoke a document function.
	pos = ::AfxGetApp() ->GetFirstDocTemplatePosition();
	if(pos == NULL)
	{
		MessageBox(_T("Create a document first please."));
		return;
	}
	CDocTemplate *pTemplate = AfxGetApp() ->GetNextDocTemplate(pos);
	pTemplate ->OpenDocumentFile(filename);
}

void CManagerDlg::OnBnClickedRemove()
{
	// TODO: Add your control notification handler code here
	CString xmldoc = m_managerXml.LoadXmlFile(FILE_DATABASE_NAME);
	m_managerXml.SetDoc(xmldoc);
	// Delete the selected item. so get the exact file name
	POSITION pos = m_fileList.GetFirstSelectedItemPosition();
	UINT nColumn = m_fileList.GetNextSelectedItem(pos);
	CString selectedFile = m_fileList.GetItemText(nColumn - 1 , 1);
	m_fileList.DeleteItem(nColumn);

	// Delete the node from the xml strings.
	m_managerXml.DeleteChildNode(_T("Video"),  _T("videoFileName"), _T("hh.avi"));

	//	Now we got the filename so
	// try to remove the file data from the XML strings.
	while (m_managerXml.FindChildElem(_T("Video")))
	{
		CString filename, filetype;
		filename = m_managerXml.GetChildAttrib(_T("videoFileName"));
		filetype = m_managerXml.GetChildAttrib(_T("extension"));

		//	need to extract the pure filename so take out the path part.
		CString resToken = _T(""), previousToken = _T("");
		INT curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = filename.Tokenize(_T("\\"), curPos);
		}

		// Now previous Token == the exact file name

		if(previousToken.CompareNoCase(selectedFile) == 0)
		{	// if the file already exists in the database.

			// Now Remove!!
//			bExisted = TRUE;
//			m_managerXml.m_
			m_managerXml.DeleteChildNode(_T("Video"), _T("videoFileName"), previousToken);
		}
	}
}
