// TTVSTestView.h : interface of the CTTVSTestView class
//


#pragma once

// Forward class reference
class TalkingHead;
class AnimationPlayer;

class CTTVSTestView : public CView
{
protected: // create from serialization only
	CTTVSTestView();
	DECLARE_DYNCREATE(CTTVSTestView)

// Attributes
public:
	CTTVSTestDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTTVSTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    BOOL LoadTalkingHead(LPCTSTR lpszPathName);
    BOOL CloseTalkingHead();

protected:
    TalkingHead     *m_pTalkingHead;
    AnimationPlayer *m_pPlayer;
    CString          m_strModelPath;
	BOOL             m_bDragging;
    BOOL             m_bCantonese;
    BOOL             m_bEnglish;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
    afx_msg void OnGeneratFLV();
    afx_msg void OnTTVSCantonese();
    afx_msg void OnTTVSEnglish();
    afx_msg void OnTTVSAnimation();
    afx_msg void OnTTVSPause();
    afx_msg void OnTTVSStop();
    afx_msg void OnUpdateCommandUI(CCmdUI *pCmdUI);
	afx_msg void OnHeadmodelCommand(UINT nID);
	afx_msg void OnHeadmodelUpdateUI(CCmdUI *pCmdUI);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in TTVSTestView.cpp
inline CTTVSTestDoc* CTTVSTestView::GetDocument() const
   { return reinterpret_cast<CTTVSTestDoc*>(m_pDocument); }
#endif

