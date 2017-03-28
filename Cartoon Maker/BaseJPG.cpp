// BaseJPG.cpp: implementation of the CBaseJPG class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CartoonMaker.h"
#include "BaseJPG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef	BASE_JPG_CPP__
#define	BASE_JPG_CPP__


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseJPG::CBaseJPG()
{
}

CBaseJPG::~CBaseJPG()
{
}

BOOL CBaseJPG::LoadJpg(LPCSTR szFile)
{
	m_pJpg = new CJpeg;
	m_pJpg ->LoadJPG((LPSTR)szFile);

	SIZE size;
	size.cx = m_pJpg ->GetWidth();
	size.cy = m_pJpg ->GetHeight();
	Create(size, 0, RGB(0,0,0));
	INT	nWidth = (size.cx * 3 + 3)/ 4 * 4;
	memcpy(m_pDib, m_pJpg ->m_pData, nWidth * size.cy);
	delete	m_pJpg;
	m_pJpg = NULL;

	return	TRUE;
}

void CBaseJPG::FlipY()
{
	INT	N = 3;
	INT RealWidth = (GetSize().cx * N + 3) / 4 * 4;
	INT	nHeight = GetSize().cy;

	INT		i;
	BYTE *pLine = new BYTE(RealWidth * nHeight);
	for(i = 0 ; i < nHeight / 2 ; i++)
	{
		memcpy(pLine, &m_pDib[i * RealWidth], RealWidth);
		memcpy(&m_pDib[i * RealWidth] , &m_pDib[(nHeight - 1 - i)
			* RealWidth], RealWidth);
		memcpy(&m_pDib[(nHeight - 1 - i) *RealWidth], pLine,
			RealWidth);
	}
	delete	[] pLine;
}

BOOL CBaseJPG::SaveJpg(LPCSTR szFile)
{
	m_pJpg = new CJpeg;
	FlipY();
	m_pJpg ->SaveJPG((LPSTR)szFile, GetSize().cx, GetSize().cy,
		(BYTE *)m_pDib);
	FlipY();
	delete	m_pJpg;
	m_pJpg = NULL;
	return	TRUE;
}

#endif