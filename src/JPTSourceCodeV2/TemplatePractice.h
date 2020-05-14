#pragma once

// policies interface
template <class T>
struct OpNewCreator
{
	static T* Creator()
	{
		return new T;
	}
};

template <class T>
struct MallocCreator
{
	static T* Create()
	{
		void* buf = std::malloc(sizeof(T));
		if (!buf) return 0;
		return new(buf) T;
	}
};

template <class T>
struct PrototypeCreator
{
	PrototypeCreator(T* pObj = 0)
		:pPrototype_(pObj)
	{}
	T* Create()
	{
		return pPrototype_ ? pPrototype_->Clone() : 0;
	}
	T* GetPrototype() { return pPrototype_; }
	void SetPrototype(T* pObj) { pPrototype_ = pObj; }
private:
	T* pPrototype_;
};
//The classes that use one or more policies are called hosts or host classes
// Library code 
//host class
template <class CreationPolicy>
class WidgetManager : public CreationPolicy
{
public:
	WidgetManager(){}
	~WidgetManager() {}
};

class Widget
{
public:
	Widget() { ; }
	void GetNothing() { ; }
};

// Library code
template <template <class Created> class CreationPolicy>
class WidgetManager2 : public CreationPolicy<Widget>
{
public:
	WidgetManager2() {}
	~WidgetManager2() {}
};

// Application code
typedef WidgetManager< PrototypeCreator<Widget>> MyWidgetMgr;
typedef WidgetManager2<PrototypeCreator> MyWidgetMgr2;