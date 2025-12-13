#include "Account.h"
#include <stdexcept>

int Account::s_ANGenerator = 1000;
Account::Account(const std::string &name, float balance, int accountNo) :
	m_Name(name), m_Balance(balance) {
	if (accountNo >= 0) {
		m_AccNo = accountNo;
		SyncAccountNumber(accountNo);
	}
	else {
		m_AccNo = ++s_ANGenerator;
	}
}

const std::string Account::GetName() const {
	return m_Name;
}

float Account::GetBalance() const {
	return m_Balance;
}

int Account::GetAccountNo() const {
	return m_AccNo;
}

bool Account::IsClosed() const {
	return m_Closed;
}

void Account::AccumulateInterest() {
	if (m_Closed) {
		throw std::runtime_error("Account is closed");
	}
}

void Account::Withdraw(float amount) {
	/*
	Balance should be greater than 0 & the amount
	to withdraw should be less than balance
	*/
	if (m_Closed) {
		throw std::runtime_error("Account is closed");
	}
	if (amount < m_Balance && m_Balance > 0)
		m_Balance -= amount;
	else {
		//Throw an exception instead of printing a message
		//std::cout << "Insufficient balance" << std::endl;
		throw std::runtime_error("Insufficient balance");
	}
}

void Account::Deposit(float amount) {
	if (m_Closed) {
		throw std::runtime_error("Account is closed");
	}
	m_Balance += amount;
}

float Account::GetInterestRate() const {
	return 0.0f;
}

void Account::Close() {
	m_Closed = true;
}

void Account::Reopen() {
	m_Closed = false;
}

void Account::SyncAccountNumber(int lastIssued) {
	if (lastIssued > s_ANGenerator) {
		s_ANGenerator = lastIssued;
	}
}
