#include "Savings.h"
#include <iostream>

// Inheritance: initialize base Account part of Savings.
Savings::Savings(const std::string & name, float balance, float rate, int accountNo):Account(name, balance, accountNo), m_Rate(rate) {
	//std::cout << "Savings(const std::string &, float)" << std::endl;
}

float Savings::GetInterestRate() const {
	return m_Rate;
}

void Savings::AccumulateInterest() {
	m_Balance += (m_Balance * m_Rate);
}
