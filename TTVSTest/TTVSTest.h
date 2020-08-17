// TTVSTest.h : main header file for the TTVSTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTTVSTestApp:
// See TTVSTest.cpp for the implementation of this class
//

class CTTVSTestApp : public CWinApp
{
public:
	CTTVSTestApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTTVSTestApp theApp;