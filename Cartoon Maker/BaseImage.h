// BaseImage.h: interface for the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEIMAGE_H__410E2241_EC5C_11D6_A4DC_00010297914C__INCLUDED_)
#define AFX_BASEIMAGE_H__410E2241_EC5C_11D6_A4DC_00010297914C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PALVERSION 0x300
#define DIB_HEADER_MARKER ((WORD)('M'<<8) | 'B')

class CBaseImage  : public CObject
{
protected:
	BITMAPFILEHEADER bmpHeader;
	LPBITMAPINFO pBitmapInfo;
	LPSTR m_pDib;
	LPSTR tDib;
	int m_ndumy;
	int m_nColor;
	LPLOGPALETTE lpLogPalette;
	CPalette *cPalette;
	CString m_strFileName;
public:
	CBitmap *m_pBitmap;
	CBaseImage();
	virtual ~CBaseImage();

public:
	// ���� �Լ�ȭ �� ������ �����غ���..
	virtual BOOL LoadJpg(CString) {
		return	TRUE;
	};
	void MakeBitmap();
	BOOL SaveImage(CString *file=NULL);
	/*
	Create
	size ũ���� ������ colornum�� ������ ����� backcolor�� �����.
	*/
	BOOL Create(SIZE size, int colornum,COLORREF backcolor);
	void Create(int Width, int Height, COLORREF * pPal);
	/* 
	GetSize:
	�̹����� ũ�⸦ �ȴ�.
	*/
	SIZE GetSize();
	/*
	PaintImage
	ȭ�鿡 �̹����� ����Ѵ�.
	*/
	BOOL  PaintImage(int x, int y, HDC hDC);
	/*
	LoadBmp
	filename���� bmp�� �ε��Ѵ�.
	*/
	BOOL LoadBmp(CString filename);
	/*
	SaveImage
	bmp ������ �����Ѵ�.
	*/
	BOOL SaveImage(CString file);
	//���ϸ� ���
	CString GetFileName(){return m_strFileName.GetBuffer();}
	//���� Į��� ���
	int GetColorNum(){	return m_nColor;}
	//�ȷ�Ʈ ���
	CPalette * GetPalette(){return cPalette;}
	LPBITMAPINFO GetBitmapInfo(){return pBitmapInfo;};


protected:
	/*
	SetBmpInfo:
		szBitmapInfo�� ���� �����ϴ� �Լ�
		������: 16Į�� 2Į�� ���� ���� 4�� 8���� ���� �̹��� �����ȵ�
	*/
	BOOL SetBmpInfo(SIZE size, int colornum);
	/*
	AllocBmpInfo
	pBitmapInfo�� �޸𸮿� �����ϴ� �Լ�
	*/
	BOOL AllocBmpInfo(int colornum);
	/*
	CreatePalette
	mode�� 1�̸� pBitmapInfo ����
	�׷��� ������ ����
	*/

	BOOL CreatePalette(BOOL mode);
	/*
	FindColorInPalette
	�ȷ�Ʈ �ȿ��� fi �� ���� �ִ°��� ã�´�.
	version up: ����� ������ ����
	*/
	int FindColorInPalette(COLORREF fi);
	/*
	SetSystemPalette
	�����쿡 �����Ǿ��ִ� �ȷ�Ʈ�� ����Ѵ�.
	*/
	BOOL SetSystemPalette(HDC hDC);

};

#endif // !defined(AFX_BASEIMAGE_H__410E2241_EC5C_11D6_A4DC_00010297914C__INCLUDED_)
