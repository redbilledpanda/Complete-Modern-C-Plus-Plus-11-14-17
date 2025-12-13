#include "Integer.h"
#include <memory>
void Display(Integer *p) {
	if(!p) {
		return ;
	}
	std::cout << p->GetValue() << std::endl; 
}

std::unique_ptr<Integer> GetPointer(int value) {
		return std::make_unique<Integer>(value);
}

void Store(std::unique_ptr<Integer> &p) {
	std::cout << "Storing data into a file:" << p->GetValue() << std::endl; 
}

void Operate(int value) {
	try {
			auto p = GetPointer(value) ;
			p->SetValue(100) ;
			Display(p.get()) ;
			//delete p ;
			//p = nullptr ;
			//p = new Integer{} ;
			p = Integer::Create();
			*p = __LINE__ ;
			Display(p.get()) ;
			Store(p) ;
			//delete p ;
	} catch(const std::bad_alloc &ex) {
			std::cerr << "Memory allocation failed: " << ex.what() << std::endl ;
	} catch(const std::exception &ex) {
			std::cerr << "An error occurred: " << ex.what() << std::endl ;
	}
}

int main() {
	Operate(5) ;
}