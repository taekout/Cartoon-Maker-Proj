// CartoonMakerView.h : CCartoonMakerView Ŭ������ �������̽�
//


#pragma once


class CCartoonMakerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CCartoonMakerView();
	DECLARE_DYNCREATE(CCartoonMakerView)

// Ư���Դϴ�.
public:
	CCartoonMakerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// �����Դϴ�.
public:
	virtual ~CCartoonMakerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAviLine();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAviCartoonmaking();
	afx_msg void OnConvertOneFrameToManga();
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // CartoonMakerView.cpp�� ����� ����
inline CCartoonMakerDoc* CCartoonMakerView::GetDocument() const
   { return reinterpret_cast<CCartoonMakerDoc*>(m_pDocument); }
#endif

