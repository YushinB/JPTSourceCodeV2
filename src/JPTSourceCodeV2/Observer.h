#pragma once
/********************************************//**
*  ... CObserver
***********************************************/

class CSubject;

class CObserver
{
public:
	CObserver();
	virtual ~CObserver();

	virtual int UpdateObserver(CSubject* pSubject, UINT Msg, WPARAM wParam, LPARAM lParam);
};

