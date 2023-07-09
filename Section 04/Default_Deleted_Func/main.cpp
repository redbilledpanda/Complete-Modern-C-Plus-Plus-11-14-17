#include <iostream>
class Integer {
	int m_Value{ 0 };
public:
	/* can only be used with functions that can 
	 * be 'properly' synthesized by the compiler,
	 * like dtor, copy ctor and assignment operator */
	Integer() = default;
	//Integer() {
	//	m_Value = 0;
	//}
	Integer(int value) {
		m_Value = value;
	}
	Integer(const Integer &) = delete;
	void SetValue(int value) {
		m_Value = value;
	}
	void SetValue(float) = delete;
};

int main() { 
	Integer i1; 
	i1.SetValue(5);
	//Error as SetValue(float) is deleted
	i1.SetValue(67.1f);
	return 0;
}