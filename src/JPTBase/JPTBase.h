// JPTBase.h : main header file for the JPTBase DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CJPTBaseApp
// See JPTBase.cpp for the implementation of this class
//

class CJPTBaseApp : public CWinApp
{
public:
	CJPTBaseApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
