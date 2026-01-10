#pragma once
#include "Account.h"
// Inheritance: Savings derives from Account and specializes interest behavior.
class Savings : public Account 
{
	float m_Rate; // Encapsulation: savings-specific state is private to the class.
public:
	Savings(const std::string &name, float balance, float rate, int accountNo = -1);
	~Savings() override = default;
	float GetInterestRate()const override;
	void AccumulateInterest() override;
};
