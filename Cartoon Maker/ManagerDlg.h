#pragma once
#include "afxcmn.h"
// Don't include a header file from a header file. might cause a intertwined link error
//#include "xmlparser.h"
extern class CXmlParser;

// CManagerDlg dialog

class CManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CManagerDlg)

public:
	CManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManagerDlg();

// Dialog Data
	enum { IDD = IDD_FILEMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTest();
public:
	// It manages a file list
	CListCtrl m_fileList;
public:
	afx_msg void OnLvnItemchangedFilelist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	virtual BOOL OnInitDialog();
public:
	CXmlParser m_managerXml;
public:
	afx_msg void OnBnClickedDisplay();
public:
	afx_msg void OnBnClickedRemove();
};
