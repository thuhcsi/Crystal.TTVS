// TTVSTestDoc.cpp : implementation of the CTTVSTestDoc class
//

#include "stdafx.h"
#include "TTVSTest.h"

#include "TTVSTestDoc.h"
#include "TTVSTestView.h"
#include ".\ttvstestdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTTVSTestDoc

IMPLEMENT_DYNCREATE(CTTVSTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTTVSTestDoc, CDocument)
END_MESSAGE_MAP()


// CTTVSTestDoc construction/destruction

CTTVSTestDoc::CTTVSTestDoc()
{
	// TODO: add one-time construction code here

}

CTTVSTestDoc::~CTTVSTestDoc()
{
}

// CTTVSTestDoc serialization

void CTTVSTestDoc::Serialize(CArchive& ar)
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


// CTTVSTestDoc diagnostics

#ifdef _DEBUG
void CTTVSTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTTVSTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTTVSTestDoc commands

BOOL CTTVSTestDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    CTTVSTestView * pView = reinterpret_cast<CTTVSTestView*>(m_viewList.GetHead());
    pView->CloseTalkingHead();

    return TRUE;
}

BOOL CTTVSTestDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  Add your specialized creation code here
    CTTVSTestView * pView = reinterpret_cast<CTTVSTestView*>(m_viewList.GetHead());
    pView->CloseTalkingHead();
    if (!pView->LoadTalkingHead(lpszPathName))
        return FALSE;

    return TRUE;
}
