#pragma once


// CDrawDlg dialog
#include "BitmapSlider.h"
#include "afxwin.h"
#include "BitmapEx.h"

class CDrawDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawDlg)

public:
	CDrawDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDrawDlg();

// Dialog Data
	enum { IDD = IDD_FINETWEAKDLG };

private:
	CMDIFrameWnd * m_pFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()



public:
	COLORREF   m_color;
	CString    m_filename;
	CBitmapEx *m_pStaticBitmap;
	BOOL       m_bSetPixel;
	BOOL       m_bDrawLine;
	LPVOID * m_pDoc;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnInitDialog();
	void SetMainFramePointer(CMDIFrameWnd * pFrame)
	{
		m_pFrame = pFrame;
	}

	afx_msg void OnBnClickedDrawLine();
	afx_msg void OnBnClickedLuminance();
	afx_msg void OnBnClickedSetcolor();
	afx_msg void OnBnClickedBlur();
	afx_msg void OnBnClickedEmboss();
	afx_msg void OnBnClickedGreyscale();
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnClickedPixel();
public:
	afx_msg void OnBnClickedSharp();
public:
	afx_msg void OnBnClickedPen();
public:
	afx_msg void OnBnClickedFloodfill();
public:
	afx_msg void OnBnClickedSave();
public:
	afx_msg void OnBnClickedGetcolor();
};
#pragma once


// CSliderDlg dialog

class CSliderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSliderDlg)

public:
	CSliderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSliderDlg();
	// Document pointer
	LPVOID     m_pDoc;


// Dialog Data
	enum { IDD = IDD_SLIDERDLG };

	// Bus Slider
	CBitmapSlider	m_sliderBus;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnSliderMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSliderMoved(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CString m_strStatic;
public:
	afx_msg void OnStnClickedSliderstatic();
};
