#ifndef XMLPARSER_CLASS
#define XMLPARSER_CLASS

#pragma once
#include "Xml.h"
//extern class CMarkup;

class CXmlParser :
	public CMarkup
{
public:
	CXmlParser(void);
public:
	~CXmlParser(void);

public:
	/// attributes
	CString subtitle;
	CFile *pFile;
	CMarkup *m_pXmlCaption;
	CMarkup  m_fileManager;


public:
	// Operations
	HRESULT LoadFile(CString filename);
	CString CreateFileManager(void);
public:
	// Should be static
	CString  LoadXmlFile(CString filename);
	CString ReadSub(CString subfilename, int currentSync = 0);

public:
	HRESULT DeleteChildNode(CString nodename,CString attirubeName, CString attributeText);
};


#endif