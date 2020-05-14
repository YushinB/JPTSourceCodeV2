#pragma once 

class CBase
{
public:
	CBase(){ f("base constructor"); }
	~CBase(){ f("base destructor"); }

	virtual const char* v() { return "base::v()"; }
	void f(const char* caller) { 
		std::cout << "When called from " << caller << ", "  << v() << " gets called.\n"; 
	}
};

class CDerived : public CBase
{
public:
	CDerived(){ f("derived constructor"); }
	~CDerived(){ f("derived destructor"); }


	const char* v() override { return "derived::v()"; }
};

namespace Test
{
	void TestMain();
}
