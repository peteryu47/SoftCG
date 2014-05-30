
// SoftCG.h : main header file for the SoftCG application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSoftCGApp:
// See SoftCG.cpp for the implementation of this class
//

class CSoftCGApp : public CWinApp
{
public:
	CSoftCGApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSoftCGApp theApp;
