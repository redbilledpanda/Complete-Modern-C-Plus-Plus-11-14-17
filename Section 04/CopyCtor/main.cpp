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

/* even though the func is empty, we see that 
 * the object is passed by value which neccesiates 
 * invoking the copy ctor */
void Process(Integer val) {

}

int main() {
	//Integer a(3);

	/* lets call the delegating ctor */
	Integer b;
	
	/*auto b(std::move(a)) */
	
	/* this should invoke the copy ctor to create
	 * a temp object for the sake of calling process */
	Process(std::move(b));
	return 0;
}