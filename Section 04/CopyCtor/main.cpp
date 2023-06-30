#include "Integer.h"
#include <iostream>
#include <memory>
class IntPtr {
	Integer *m_p;
public:
	IntPtr(Integer *p) :m_p(p) {

	}
	~IntPtr() {
		delete m_p;
	}
	Integer *operator ->() {
		return m_p;
	}
	Integer & operator *() {
		return *m_p;
	}
};
void CreateInteger() {
	std::unique_ptr<Integer> p(new Integer);
	//auto p2(p);
	(*p).SetValue(3);
	//std::cout << p->GetValue() << std::endl; 
}

/* @brief this function is a placeholder to demonstrate 
 * that NOT having a default copy ctor  can create pointer 
 * dereference issues as it manipulates the same copy in mem */
void Process(Integer val) {

}

Integer Add(int x, int y){
	return Integer(x+y);
}

int main() {
	Integer a(3);
	//Integer b(a);

	//Integer c = (a.GetValue(), 5);

	/*auto b(std::move(a));*/
	std::cout << a.GetValue() << std::endl;
	Process(std::move(a));
	return 0;
}

