#include "Checking.h"

#include <iostream>
// Inheritance: initialize base Account part of Checking.
Checking::Checking(const std::string &name, float balance, float minbalance, int accountNo):
m_MinimumBalance(minbalance), Account(name, balance, accountNo){
}

void Checking::Withdraw(float amount) {
	if ((m_Balance - amount) > m_MinimumBalance) {
		// Inheritance: reuse base class withdrawal rules.
		Account::Withdraw(amount);
	}
	else {
		std::cout << "Invalid amount" << std::endl; 
	}
}

float Checking::GetMinimumBalance() const {
	return m_MinimumBalance;
}
