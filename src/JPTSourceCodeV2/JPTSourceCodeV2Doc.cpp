// JPTSourceCodeV2Doc.cpp : implementation file
//

#include "stdafx.h"
#include "JPTSourceCodeV2.h"
#include "JPTSourceCodeV2Doc.h"


// JPTSourceCodeV2Doc

IMPLEMENT_DYNCREATE(JPTSourceCodeV2Doc, CDocument)

JPTSourceCodeV2Doc::JPTSourceCodeV2Doc()
{
}

BOOL JPTSourceCodeV2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

JPTSourceCodeV2Doc::~JPTSourceCodeV2Doc()
{
}


BEGIN_MESSAGE_MAP(JPTSourceCodeV2Doc, CDocument)
END_MESSAGE_MAP()


// JPTSourceCodeV2Doc diagnostics

#ifdef _DEBUG
void JPTSourceCodeV2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void JPTSourceCodeV2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// JPTSourceCodeV2Doc serialization

void JPTSourceCodeV2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// JPTSourceCodeV2Doc commands
