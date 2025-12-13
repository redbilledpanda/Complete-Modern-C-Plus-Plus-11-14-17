#pragma once
#include <string>
class Account {
	std::string m_Name;
	int m_AccNo;
	static int s_ANGenerator;
	bool m_Closed{false};
protected:
	float m_Balance;
public:
	Account(const std::string &name, float balance, int accountNo = -1);
	virtual ~Account() = default;
	[[nodiscard]] const std::string GetName()const;
	[[nodiscard]] float GetBalance()const;
	[[nodiscard]] int GetAccountNo()const;
	[[nodiscard]] bool IsClosed() const;

	virtual void AccumulateInterest();
	virtual void Withdraw(float amount);
	void Deposit(float amount);
	[[nodiscard]] virtual float GetInterestRate()const;
	void Close();
	void Reopen();
	static void SyncAccountNumber(int lastIssued);
};
