// CartoonMaker.h : CartoonMaker ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CCartoonMakerApp:
// �� Ŭ������ ������ ���ؼ��� CartoonMaker.cpp�� �����Ͻʽÿ�.
//

class CCartoonMakerApp : public CWinApp
{
public:
	CCartoonMakerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCartoonMakerApp theApp;