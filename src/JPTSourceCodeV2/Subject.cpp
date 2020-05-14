#include "stdafx.h"
#include "Subject.h"
#include <algorithm>
#include "Observer.h"

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
	ClearObservers();
}

void CSubject::AttachObserver(CObserver * pObsvr)
{
	if (pObsvr != NULL)
	{
		if (m_observer_list.end() == std::find(m_observer_list.begin(), m_observer_list.end(), pObsvr))
		{
			m_observer_list.push_back(pObsvr);
		}
	}
}

void CSubject::DettachObserver(CObserver * pObsvr)
{
	if (pObsvr != NULL)
	{
		m_observer_list.remove(pObsvr);
	}
}

void CSubject::ClearObservers()
{
	m_observer_list.clear();
}

int CSubject::Notify(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	container_type::iterator itr;
	for (itr = m_observer_list.begin(); itr != m_observer_list.end(); ++itr)
	{
		int res = (*itr)->UpdateObserver(this, Msg, wParam, lParam);
		if (res != 0) return res;
	}
	return 0;
}
