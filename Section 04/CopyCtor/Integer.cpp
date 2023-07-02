#include "Integer.h"
#include <iostream>

/* @brief delegating ctor */
Integer::Integer():Integer(0) {
	std::cout << "Integer() working on object " << this << "." << std::endl;
}

Integer::Integer(int value) {
	std::cout << "Integer(int), working on object " << this << "." << std::endl;
	m_pInt = new int(value);
}

Integer::Integer(const Integer & obj) {
	std::cout << "Integer(const Integer&) working on object " << this << "." << std::endl;
	m_pInt = new int(*obj.m_pInt);
}


int Integer::GetValue() const {
	return *m_pInt;
}

void Integer::SetValue(int value) {
	*m_pInt = value;
}

Integer::~Integer() {
	std::cout << "~Integer() working on object " << this << "." << std::endl;
	delete m_pInt;
}
