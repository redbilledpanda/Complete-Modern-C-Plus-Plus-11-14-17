#pragma once
#include "Account.h"
class Checking :
	public Account {
	float m_MinimumBalance;
public:
	using Account::Account;
	Checking(const std::string &name, float balance, float minbalance, int accountNo = -1);
	~Checking() override = default;
	void Withdraw(float amount)override;
	[[nodiscard]] float GetMinimumBalance()const;
};
