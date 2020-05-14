/********************************************//**
*  ... CObserver
***********************************************/

#include "stdafx.h"`
#include "Observer.h"

CObserver::CObserver()
{
}

CObserver::~CObserver()
{
}

int CObserver::UpdateObserver(CSubject * pSubject, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// return value
	// 0		: continuously update next observer
	// not 0	: Stop updating observer, and return value 
	return 0;
}