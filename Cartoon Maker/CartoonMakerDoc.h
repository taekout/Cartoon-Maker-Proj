// CartoonMakerDoc.h : CCartoonMakerDoc Ŭ������ �������̽�
//


#pragma once

#include "DrawDlg.h"


typedef struct chosenFrames
{
	UINT		numberOfChosenFrames;
	CUIntArray	listChosenFrames;
	chosenFrames()
	{
		numberOfChosenFrames = 0;
	}
} chosenFrames;

class CCartoonMakerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CCartoonMakerDoc();
	DECLARE_DYNCREATE(CCartoonMakerDoc)

// Ư���Դϴ�.
public:
	CDrawDlg *m_pDrawDlg;
	CSliderDlg *m_pSliderDlg;
	CvCapture * capture;
	IplImage * frame[10000];
	int cnt_frame,play_frame;
	int check_rec;
	bool check_save;
	IplImage * m_view;

	CPoint m_point[1000];
	int cnt_point;
	int state_editor;//0:Get Color 1:Pen 2:Flood Fill
	CString m_filename;
	chosenFrames m_listFrames;
	CBitmapEx m_bitmapUsedForCartoonMaking;
//	CXmlParser m_Parser;

// �۾��Դϴ�.

public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	
// �����Դϴ�.
public:
	virtual ~CCartoonMakerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileCam();
	afx_msg void OnRec();
	afx_msg void OnStop();

public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnPpmColor2grey();
	afx_msg void OnBlur();
	afx_msg void OnLuminanceBrighter();
	afx_msg void OnLuminanceDarker();
	afx_msg void OnEffectsFire();
	afx_msg void OnEffectsWater();
	IplImage * Hatching(CString filename);
	afx_msg void OnAviHatching();
	afx_msg void OnFileSaveAs();
	afx_msg void OnAviChooseframe();
	afx_msg void OnAviCartoonmakingwithchosenframes();
	afx_msg void OnSetbackgroundcolorforcartoonRed();
	afx_msg void OnSetbackgroundcolorforcartoonGreen();
	afx_msg void OnSetbackgroundcolorforcartoonBlue();
	afx_msg void OnSetbackgroundcolorforcartoonBlack();
	afx_msg void OnSetbackgroundcolorforcartoonWhite();
public:
	afx_msg void OnSetbackgroundcolorforcartoonUserdefinedcolor();
};


