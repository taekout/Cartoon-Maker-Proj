// BaseJPG.h: interface for the CBaseJPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEJPG_H__CBD264A5_F62E_11D6_A4DC_00010297914C__INCLUDED_)
#define AFX_BASEJPG_H__CBD264A5_F62E_11D6_A4DC_00010297914C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseImage.h"
#include "Jpeg.h"	// Added by ClassView

class CBaseJPG : public CBaseImage  
{
public:
	BOOL SaveJpg(LPCSTR szFile);
	void FlipY(void);
	BOOL LoadJpg(LPCSTR file);
	CBaseJPG();
	virtual ~CBaseJPG();

protected:
	CJpeg *m_pJpg;
};

#endif // !defined(AFX_BASEJPG_H__CBD264A5_F62E_11D6_A4DC_00010297914C__INCLUDED_)
