// BaseBitmap.cpp: implementation of the CBaseBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CartoonMaker.h"
#include "BaseBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef	BASE_BITMAP_CPP__
#define	BASE_BITMAP_CPP__


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseBitmap::CBaseBitmap()
{

}

CBaseBitmap::~CBaseBitmap()
{

}

BOOL CBaseBitmap::LoadJpg(LPCSTR)
{
	return	FALSE;
}

#endif