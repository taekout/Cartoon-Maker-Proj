#include "StdAfx.h"
#include "XmlParser.h"

CXmlParser::CXmlParser(void)
{
	pFile = NULL;
	m_pXmlCaption = NULL;
}

CXmlParser::~CXmlParser(void)
{
}

HRESULT CXmlParser::LoadFile(CString filename)
{
	pFile = new CFile(filename, CFile::modeRead);
	subtitle = filename;


	pFile ->Close();

	return E_NOTIMPL;
}

CString CXmlParser::ReadSub(CString subfilename, int currentSync)
{
/*

From this point on, the testing source starts.

*/
	CFile *pFile = new CFile(subfilename, CFile::modeRead);

	CStringArray string_arr;
	CString buf;
	char pBuf[600] = {0};
	LONG fileLength = pFile ->GetLength();
	INT i  = 0, j;
#define BUFFER_ATONCE 500
	for(i = 0; i < fileLength ; i+= BUFFER_ATONCE)
	{
		for(j = 0 ; j < 600 ; j++)
		{
			pBuf[j] = 0;
		}
		pFile ->Read(pBuf, BUFFER_ATONCE);
		buf = pBuf;
		string_arr.Add(buf);
	}
	string_arr.Add(buf);

	pFile ->Close();
	i = string_arr.GetCount();
	j = 0;
	buf = "";	// The initialization of BUF
	for(j = 0 ; j < i ; j++)
	{
		buf += string_arr.GetAt(j);
	}

	INT curPos = 0;
	CString resToken = _T("");
	resToken = buf.Tokenize(_T("\n"), curPos);
	resToken.Trim(' ');
	if(resToken == _T(""))
	{
		::AfxMessageBox(_T("The caption file is empty. Please check it out."));
	}
	m_pXmlCaption = new CMarkup();
	m_pXmlCaption ->AddElem(_T("Captions"));
	m_pXmlCaption ->SetAttrib(_T("description"), _T("The caption data of the file"));
	m_pXmlCaption ->SetAttrib(_T("noOfCaptions"), _T("N/A Later"));
	m_pXmlCaption ->SetAttrib(_T("version"), _T("0.1"));
	m_pXmlCaption ->SetAttrib(_T("videoFileName"),_T("N/A Later"));
/*
CMarkup xml;
xml.AddElem( "Captions" );
xml.AddChildElem( "Caption" );
xml.IntoElem
();
xml.AddChildElem( "SN", "132487A-J" );
xml.AddChildElem( "NAME", "crank casing" );
xml.AddChildElem( "QTY", "1" );
CString csXML = xml.GetDoc();

*/
	// convert the first string to a caption No.
	while(curPos != -1)
	{
		//	Here I convert the string to XML element 
		//	<Caption captionNo=resToken_data fontstyle="Arial">
		m_pXmlCaption ->AddChildElem(_T("Caption"));
		m_pXmlCaption ->AddChildAttrib(_T("captionNo"), resToken);
		m_pXmlCaption ->AddChildAttrib(_T("fontstyle"), _T("Arial"));


		// This string should be sync info.
		/* Some code to convert it to XML Sync info */
		resToken = buf.Tokenize(_T("\n"), curPos);
		m_pXmlCaption ->IntoElem();
		m_pXmlCaption ->AddChildElem(_T("Sync"));
		m_pXmlCaption ->IntoElem();
		m_pXmlCaption ->AddChildElem(_T("start"), resToken.Left(12));
		m_pXmlCaption ->AddChildElem(_T("end"), resToken.Mid(17,12));


		// This string should be subtitle content.
		resToken = buf.Tokenize(_T("\n"), curPos);
		// Check if it's still in the caption.
		// Bug!!
		CString tmp = _T("Temporary token");
		while(((INT)tmp.GetAt(0)) != 13 && tmp != "")
		{// There can still be more subtitle content for many lines.
			// While in the caption, keep getting more strings.
			tmp = buf.Tokenize(_T("\n"), curPos);
			resToken += tmp;
		}
		m_pXmlCaption ->OutOfElem();
		m_pXmlCaption ->AddChildElem(_T("font"));
		m_pXmlCaption ->IntoElem();
		m_pXmlCaption ->AddChildElem(_T("location"), _T("5"));
		m_pXmlCaption ->AddChildElem(_T("size"), _T("15"));
		m_pXmlCaption ->AddChildElem(_T("content"), resToken);
		m_pXmlCaption ->OutOfElem();
		m_pXmlCaption ->OutOfElem();
		// At this point, the pointer points to the '\n'. So the next line should start
		// a new caption No.
		// It also could point to "". which means the end of file.
		if(curPos != -1)
			resToken = buf.Tokenize(_T("\n"), curPos);

	}

	CString result = m_pXmlCaption ->GetDoc();
	CFile savfile(_T("Caption_save.xml"), CFile::modeCreate | CFile::modeWrite);
	char resultBuf[1000000];
	::WideCharToMultiByte(CP_ACP, 0, result, -1, resultBuf, sizeof(resultBuf), NULL,NULL);
	savfile.Write(resultBuf, result.GetLength());

	return	result;
}



CString CXmlParser::CreateFileManager(void)
{
	m_fileManager.AddElem(_T("Videos"));
	CFile *file;
	file = new CFile(FILE_DATABASE_NAME, CFile::modeCreate | CFile::modeWrite);
	CString tmp = m_fileManager.GetDoc();
	char tmpBuf[100000];
	::WideCharToMultiByte(CP_ACP, 0, tmp.GetBuffer(), -1
				, tmpBuf, sizeof(tmpBuf), NULL,NULL);
	if(tmp.CompareNoCase(_T("<Videos/>")) == 0)
	{
		strcpy(tmpBuf, "<Videos></Videos>");
	}
	file ->Write(tmpBuf, tmp.GetLength());
	file ->Close();
	delete file;
	return tmp;//tmp;
}

// Should be static
CString CXmlParser::LoadXmlFile(CString filename)
{
	// Open file
	CWaitCursor wait;
	CString csText;
	CString csPath = filename;
	CString csNotes;
	CFile *file = NULL;

	if(filename.Right(3).CompareNoCase(_T("XML")) != 0)
	{
		AfxMessageBox(_T("There has been some mistake. The input file has to be an XML file."));
	}
	try{
		char charfilename[1000];
		CFileException e;
		::WideCharToMultiByte(CP_ACP, 0, csPath, -1, charfilename, sizeof(charfilename), NULL,NULL);
		file = new CFile(filename, CFile::modeRead);
		if (file == NULL)
		{
	//		AfxMessageBox(_T("unable to open file") );
	//		DWORD dw = GetLastError();

//			CString error;;    
//			e.GetErrorMessage(error.GetBuffer(), 1000);
			TCHAR szError[1024];
			e.GetErrorMessage(szError, 1024);
			return _T("unable to open file");
		}
	}
	catch(CException *e)
	{
		UINT pHelpContext = -100;
		TCHAR error[1000];
		CString tmp;
		tmp.Format(_T("%d"), e ->GetErrorMessage(error, 1000, &pHelpContext));
		TRACE(tmp);
//		AfxMessageBox(error);
		CString str = _T("C:\\FileManager.xml was not found.");
		return 0;
		if(str.CompareNoCase(error) == 0)
		{
			this ->CreateFileManager();
			// Sometimes the file still doesn't exist. ==> Dynamic error!
			file = new CFile(filename, CFile::modeRead);
		}
	}
	int nFileLen = (int)file ->GetLength();

	// Allocate buffer for binary file data
	unsigned char* pBuffer = new unsigned char[nFileLen + 2];
	nFileLen = file ->Read( pBuffer, nFileLen );
	file ->Close();
	delete file;
	pBuffer[nFileLen] = '\0';
	pBuffer[nFileLen+1] = '\0'; // in case 2-byte encoded

	// Windows Unicode file is detected if starts with FEFF
	if ( pBuffer[0] == 0xff && pBuffer[1] == 0xfe )
	{
		// Contains byte order mark, so assume wide char content
		// non _UNICODE builds should perform UCS-2 (wide char) to UTF-8 conversion here
		csText = (LPCWSTR)(&pBuffer[2]);
		csNotes += _T("File starts with hex FFFE, assumed to be wide char format. ");
	}
	else
	{
		// _UNICODE builds should perform UTF-8 to UCS-2 (wide char) conversion here
		csText = (LPCSTR)pBuffer;
	}
	delete [] pBuffer;

	// If it is too short, assume it got truncated due to non-text content
	if ( csText.GetLength() < nFileLen / 2 - 20 )
	{
		AfxMessageBox( _T("Error converting file to string (may contain binary data)") );
		return _T("Error converting file to string (may contain binary data)");
	}

	// Parse
//	m_fileManager = new CMarkup();
//	m_fileManager ->SetDoc(csText);
//	this ->SetDoc(
	this ->SetDoc(csText);
	this ->m_fileManager.SetDoc(csText);
	this ->ResetPos();
	this ->m_fileManager.ResetPos();

	return csText;
}

HRESULT CXmlParser::DeleteChildNode(CString nodename,
									CString attributeName,
									CString attributeText)
{
/*	CString tmp;
	tmp = this ->GetDoc();

	INT curPos = 0;
	tmp.Find(_T("<") + nodename + _T(" ") + attributeName + _T("=\""), curPos);

	while(1)
	{
		tmp.Find(_T("\""), curPos);
	} 
	CString manipulated = _T("<") + tmp + _T(" ") + attributeName + _T("=") + attributeText;
//<Video videoFileName="C:\Documents and Settings\Owner\Desktop\CartoonMaker\hh.avi" extension="avi" title="C:\Documents and Settings\Owner\Desktop\CartoonMaker\hh.avi">
	INT start = tmp.Find(manipulated , curPos);
	manipulated = _T("</") + nodename + _T(">");
	INT end = tmp.Find(manipulated, curPos);


	return	TRUE;
*/
	CXmlParser tmpXml;

	BOOL bFinished = FALSE;
	this ->ResetPos();
	this ->m_fileManager.ResetPos();

	CString currentNode = this ->GetTagName();


	if ( ! this ->FindChildElem() )
	    bFinished = TRUE;
	// Right now the root node - <Videos>
	currentNode = this ->GetTagName();
	while ( ! bFinished )
	{
	    // Process element
	    this ->IntoElem();
		// The child node - <Video>
		currentNode = this ->GetTagName();
	    CString csTag = this ->GetTagName();
		CString tmpAttText = this ->GetAttrib(attributeName);
		BOOL bSaveMode = TRUE;

		//////////////
		CString resToken = _T(""), previousToken = _T("");
		INT curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = nodename.Tokenize(_T("\\"), curPos);
		}
		nodename = resToken;
		//////////////
		//////////////
		resToken = _T(""), previousToken = _T("");
		curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = csTag.Tokenize(_T("\\"), curPos);
		}
		csTag = resToken;
		//////////////
		//////////////
		resToken = _T(""), previousToken = _T("");
		curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = attributeText.Tokenize(_T("\\"), curPos);
		}
		attributeText = resToken;
		//////////////
		//////////////
		resToken = _T(""), previousToken = _T("");
		curPos = 0;
		while(curPos != -1)
		{
			previousToken = resToken;
			resToken = tmpAttText.Tokenize(_T("\\"), curPos);
		}
		tmpAttText = resToken;
		//////////////
		if(csTag.Compare(nodename) == 0 && tmpAttText.Compare(attributeText) == 0)
		{
			// Here you can delete the found node.
//			this ->d();
//			bSaveMode = FALSE;
//			CString mainDoc = this ->GetDoc();
//			CString tmp = <Video>
//			mainDoc.FindOneOf(

		}

	    // Next element (depth first)

	    BOOL bFound = this ->FindChildElem();
	    while ( ! bFound && ! bFinished )
	    {
	        if ( this ->OutOfElem() )
	            bFound = this ->FindChildElem();
	        else
	            bFinished = TRUE;
			currentNode = this ->GetTagName();
	    }
	}

	return E_NOTIMPL;
}
