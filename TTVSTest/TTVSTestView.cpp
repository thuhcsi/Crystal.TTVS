// TTVSTestView.cpp : implementation of the CTTVSTestView class
//

#include "stdafx.h"
#include "TTVSTest.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "TTVSTestDoc.h"
#include "TTVSTestView.h"

#include "../TTVS/TalkingHeadConcatenate.h"
#include "../TTVS/TalkingHead3D.h"
#include "../TTVS/AnimationPlayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTTVSTestView

IMPLEMENT_DYNCREATE(CTTVSTestView, CView)

BEGIN_MESSAGE_MAP(CTTVSTestView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_TTS_GEN_FLV, &CTTVSTestView::OnGeneratFLV)
    ON_COMMAND(ID_TTS_DISP_ANIMATION, &CTTVSTestView::OnTTVSAnimation)
    ON_COMMAND(ID_TTS_CANTONESE, &CTTVSTestView::OnTTVSCantonese)
    ON_COMMAND(ID_TTS_ENGLISH, &CTTVSTestView::OnTTVSEnglish)
    ON_COMMAND(ID_TTVS_PAUSE, &CTTVSTestView::OnTTVSPause)
    ON_COMMAND(ID_TTVS_STOP, &CTTVSTestView::OnTTVSStop)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TTS_GEN_FLV, ID_TTVS_STOP, &CTTVSTestView::OnUpdateCommandUI)
	ON_COMMAND_RANGE(ID_HEADMODEL_ZOOM, ID_HEADMODEL_ROTATE, &CTTVSTestView::OnHeadmodelCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HEADMODEL_ZOOM, ID_HEADMODEL_ROTATE, &CTTVSTestView::OnHeadmodelUpdateUI)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CTTVSTestView construction/destruction

CTTVSTestView::CTTVSTestView()
{
	// TODO: add construction code here
    m_pTalkingHead = NULL;
    m_pPlayer = NULL;
	m_bDragging = FALSE;
    m_bCantonese = FALSE;
    m_bEnglish = FALSE;
}

CTTVSTestView::~CTTVSTestView()
{
    if (m_pTalkingHead)
        m_pTalkingHead->unloadModel();
    delete m_pTalkingHead;
    delete m_pPlayer;
}

BOOL CTTVSTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTTVSTestView diagnostics

#ifdef _DEBUG
void CTTVSTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTTVSTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTTVSTestDoc* CTTVSTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTTVSTestDoc)));
	return (CTTVSTestDoc*)m_pDocument;
}
#endif //_DEBUG



// CTTVSTestView drawing

void CTTVSTestView::OnDraw(CDC* /*pDC*/)
{
    CTTVSTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    //// TODO: add draw code for native data here

    // Draw the TalkingHead window
    if (m_pTalkingHead)
        m_pTalkingHead->render();
}

// CTTVSTestView message handlers

BOOL CTTVSTestView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    // Do not erase background to prevent flickering
    return FALSE;
    //return CView::OnEraseBkgnd(pDC);
}

int CTTVSTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    HWND hWnd = GetSafeHwnd();

    // Initialize the TalkingHead window
    if (m_pTalkingHead != NULL)
        delete m_pTalkingHead;
    //m_pTalkingHead = new TalkingHeadConcatenate(hWnd);
    m_pTalkingHead = new TalkingHead3D(hWnd);
    m_pPlayer = new AnimationPlayer(m_pTalkingHead, hWnd);

    if (m_pTalkingHead == NULL || m_pPlayer == NULL)
        return -1;

    return 0;
}

void CTTVSTestView::OnDestroy()
{
    CView::OnDestroy();

    // Clear the TalkingHead window
    if (m_pTalkingHead)
        m_pTalkingHead->unloadModel();
    delete m_pTalkingHead;
    delete m_pPlayer;
    m_pTalkingHead = NULL;
    m_pPlayer = NULL;
}

void CTTVSTestView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // Resize the TalkingHead window
    if (m_pTalkingHead)
        m_pTalkingHead->cameraWindow(cx, cy);
}

BOOL CTTVSTestView::LoadTalkingHead(LPCTSTR lpszPathName)
{
    if (m_pTalkingHead == NULL)
        return FALSE;

    CString fullPathName = lpszPathName;
    int pos = fullPathName.ReverseFind(L'/');
    if (pos == -1)
        pos = fullPathName.ReverseFind(L'\\');

    CString filePath = fullPathName.Left(pos + 1);
    CString fileName = fullPathName.Right(fullPathName.GetLength() - pos - 1);
    m_strModelPath   = filePath;

    if (m_pTalkingHead->loadModel(fileName.LockBuffer(), filePath.LockBuffer()))
        return TRUE;
    else
        return FALSE;
}

BOOL CTTVSTestView::CloseTalkingHead()
{
    if (m_pTalkingHead)
        m_pTalkingHead->unloadModel();
    return TRUE;
}

void CTTVSTestView::OnGeneratFLV()
{
    // TODO: Add your command handler code here
}

void CTTVSTestView::OnTTVSAnimation()
{
    // TODO: Add your command handler code here
    CString wavFileName = L"";
    if (m_bEnglish)
    {
        // English
        m_pTalkingHead->generateAnimation(m_strModelPath+_T("/../Demo/demoEnglish.txt"), _T("en"));
        wavFileName = m_strModelPath+_T("/../Demo/demoEnglish.wav");
    }
    else if (!m_bCantonese)
    {
        // Putonghua
        m_pTalkingHead->generateAnimation(m_strModelPath+_T("/../Demo/demoPutonghua.txt"), _T("zh-cmn"));
        wavFileName = m_strModelPath+_T("/../Demo/demoPutonghua.wav");
    }
    else
    {
        // Cantonese
        m_pTalkingHead->generateAnimation(m_strModelPath+_T("/../Demo/demoCantonese.txt"), _T("zh-yue"));
        wavFileName = m_strModelPath+_T("/../Demo/demoCantonese.wav");
    }
    PlaySound(wavFileName, NULL, SND_FILENAME | SND_ASYNC);
    m_pPlayer->play();
}

void CTTVSTestView::OnTTVSPause()
{
    // TODO: Add your command handler code here
    if (m_pPlayer->getStatus() == AnimationPlayer::PAUSED)
        m_pPlayer->resume();
    else
        m_pPlayer->pause();
}

void CTTVSTestView::OnTTVSStop()
{
    // TODO: Add your command handler code here
    m_pPlayer->stop();
}

void CTTVSTestView::OnTTVSCantonese()
{
    // TODO: Add your command handler code here
    m_bCantonese = !m_bCantonese;
}

void CTTVSTestView::OnTTVSEnglish()
{
    // TODO: Add your command handler code here
    m_bEnglish = !m_bEnglish;
}

void CTTVSTestView::OnUpdateCommandUI(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    switch (pCmdUI->m_nID)
    {
    case ID_TTS_GEN_FLV:
        pCmdUI->Enable(m_pTalkingHead->isLoaded() && m_pPlayer->getStatus() == AnimationPlayer::IDLE);
        break;
    case ID_TTS_DISP_ANIMATION:
        pCmdUI->Enable(m_pTalkingHead->isLoaded() && m_pPlayer->getStatus() == AnimationPlayer::IDLE);
        break;
    case ID_TTS_CANTONESE:
        pCmdUI->Enable(m_pTalkingHead->isLoaded() && m_pPlayer->getStatus() == AnimationPlayer::IDLE);
        pCmdUI->SetCheck(m_bCantonese);
        break;
    case ID_TTS_ENGLISH:
        pCmdUI->Enable(m_pTalkingHead->isLoaded() && m_pPlayer->getStatus() == AnimationPlayer::IDLE);
        pCmdUI->SetCheck(m_bEnglish);
        break;
    case ID_TTVS_PAUSE:
        pCmdUI->Enable(m_pPlayer->getStatus() != AnimationPlayer::IDLE);
        pCmdUI->SetCheck(m_pPlayer->getStatus() == AnimationPlayer::PAUSED);
        break;
    case ID_TTVS_STOP:
        pCmdUI->Enable(m_pPlayer->getStatus() != AnimationPlayer::IDLE);
        break;
    }
}

void CTTVSTestView::OnHeadmodelCommand(UINT nID)
{
	// TODO: Add your command handler code here
	CMODE mode = ZOOM;
    switch(nID)
    {
    case ID_HEADMODEL_ROTATE: mode = ROTATE; break;
	case ID_HEADMODEL_ZOOM:   mode = ZOOM;   break;
	case ID_HEADMODEL_PAN:    mode = PAN;    break;
    }
	m_pTalkingHead->cameraModeSet(mode);
}

void CTTVSTestView::OnHeadmodelUpdateUI(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMODE mode = ZOOM;
    switch(pCmdUI->m_nID)
    {
    case ID_HEADMODEL_ROTATE: mode = ROTATE; break;
    case ID_HEADMODEL_ZOOM:   mode = ZOOM;   break;
    case ID_HEADMODEL_PAN:    mode = PAN;    break;
    }
    pCmdUI->SetCheck(m_pTalkingHead->cameraModeGet()==mode);
}

void CTTVSTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

    // camera operations
    SetCapture();
    m_bDragging = TRUE;
	if (m_pTalkingHead->isLoaded())
		m_pTalkingHead->cameraStart(point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}

void CTTVSTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pTalkingHead->isLoaded())
		m_pTalkingHead->cameraEnd();
	m_bDragging = FALSE;
    ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

void CTTVSTestView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
    if ((nFlags & MK_LBUTTON) && m_bDragging)
    {
        // Left button is down
		if (m_pTalkingHead->isLoaded())
		{
			m_pTalkingHead->cameraUpdate(point.x, point.y);
			m_pTalkingHead->render();
		}
    }

	CView::OnMouseMove(nFlags, point);
}
