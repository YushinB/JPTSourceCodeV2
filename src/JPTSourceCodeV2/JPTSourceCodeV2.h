
// JPTSourceCodeV2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CJPTSourceCodeV2App:
// See JPTSourceCodeV2.cpp for the implementation of this class
//

class CJPTSourceCodeV2App : public CWinApp
{
public:
	CJPTSourceCodeV2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

private:
	//IDC_EDIT_TEXT_TEST
};

extern CJPTSourceCodeV2App theApp;