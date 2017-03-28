// CartoonMakerView.h : CCartoonMakerView 클래스의 인터페이스
//


#pragma once


class CCartoonMakerView : public CView
{
protected: // serialization에서만 만들어집니다.
	CCartoonMakerView();
	DECLARE_DYNCREATE(CCartoonMakerView)

// 특성입니다.
public:
	CCartoonMakerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// 구현입니다.
public:
	virtual ~CCartoonMakerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // CartoonMakerView.cpp의 디버그 버전
inline CCartoonMakerDoc* CCartoonMakerView::GetDocument() const
   { return reinterpret_cast<CCartoonMakerDoc*>(m_pDocument); }
#endif

