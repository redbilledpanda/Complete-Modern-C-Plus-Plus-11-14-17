#pragma once
#include "Account.h"
// Inheritance: Checking derives from Account and reuses the base interface.
class Checking : public Account 
{
	float m_MinimumBalance; // Encapsulation: checking-specific data stays inside class.
public:
	// Inheritance: bring base constructors into the derived class.
	using Account::Account;
	Checking(const std::string &name, float balance, float minbalance, int accountNo = -1);
	~Checking() override = default;
	void Withdraw(float amount)override;
	[[nodiscard]] float GetMinimumBalance()const;
};
