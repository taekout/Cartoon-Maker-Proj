// BaseBitmap.h: interface for the CBaseBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEBITMAP_H__CBD264A4_F62E_11D6_A4DC_00010297914C__INCLUDED_)
#define AFX_BASEBITMAP_H__CBD264A4_F62E_11D6_A4DC_00010297914C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseImage.h"

class CBaseBitmap : public CBaseImage
{
public:
	CBaseBitmap();
	virtual ~CBaseBitmap();

	BOOL LoadJpg(LPCSTR);

};

#endif // !defined(AFX_BASEBITMAP_H__CBD264A4_F62E_11D6_A4DC_00010297914C__INCLUDED_)
