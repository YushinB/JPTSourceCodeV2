#pragma once

class CObserver;

class CSubject
{
public:
	CSubject(void);
	virtual ~CSubject(void);

	virtual void AttachObserver(CObserver* pObsvr);
	virtual void DettachObserver(CObserver* pObsvr);
	virtual void ClearObservers();

	virtual int Notify(UINT Msg, WPARAM wParam = NULL, LPARAM lParam = NULL);

protected:
	typedef std::list<CObserver*> container_type;
	container_type m_observer_list;
};

