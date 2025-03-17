#include "Integer.h"
#include <iostream>
#include <memory>
#include <iostream>
#include <utility>      // For std::move
#include "smartInteger.h"

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
void Process(Integer val) {

}

int main() {
#if 0	
	Integer a(3);
	/*auto b(std::move(a));
	std::cout << a << std::endl;*/
	Process(std::move(a));
#else
    std::cout << "---- Creating s1 (default constructor) ----\n";
    smartInteger s1; 
    std::cout << "s1.get() = " << s1.get() << "\n\n";

    std::cout << "---- Creating s2 (parameterized constructor) ----\n";
    smartInteger s2(42);
    std::cout << "s2.get() = " << s2.get() << "\n\n";

    std::cout << "---- Demonstrate Copy Constructor (s3 from s2) ----\n";
    smartInteger s3 = s2;  // Copy construct s3 from s2
    std::cout << "s3.get() = " << s3.get() << "\n"
              << "s2.get() = " << s2.get() << " (unchanged)\n\n";

    std::cout << "---- Demonstrate Copy Assignment (s1 = s3) ----\n";
    s1 = s3; // Copy assign
    std::cout << "s1.get() = " << s1.get() << "\n"
              << "s3.get() = " << s3.get() << " (unchanged)\n\n";

    std::cout << "---- Demonstrate Move Constructor (s4 from std::move(s2)) ----\n";
    smartInteger s4 = std::move(s2); // Move construct
    std::cout << "s4.get() = " << s4.get() << "\n"
              << "s2.get() = " << s2.get() << " (moved-from, might be 0 or unspecified)\n\n";

    std::cout << "---- Demonstrate Move Assignment (s1 = std::move(s4)) ----\n";
    s1 = std::move(s4); // Move assign
    std::cout << "s1.get() = " << s1.get() << "\n"
              << "s4.get() = " << s4.get() << " (moved-from, might be 0 or unspecified)\n\n";

    std::cout << "---- Demonstrate Modifying s1 via set() ----\n";
    s1.set(999);
    std::cout << "s1.get() = " << s1.get() << "\n";
#endif
	return 0;
}