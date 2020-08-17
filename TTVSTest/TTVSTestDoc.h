// TTVSTestDoc.h : interface of the CTTVSTestDoc class
//


#pragma once

class CTTVSTestDoc : public CDocument
{
protected: // create from serialization only
	CTTVSTestDoc();
	DECLARE_DYNCREATE(CTTVSTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CTTVSTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


