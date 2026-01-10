#pragma once
#include <string>
class Account {
	std::string m_Name;          // Encapsulation: state hidden inside the class.
	int m_AccNo;                 // Encapsulation: account number managed internally.
	static int s_ANGenerator;
	bool m_Closed{false};
protected:
	// Inheritance: derived classes can access shared balance state.
	float m_Balance;             // Encapsulation: derived classes get controlled access.
public:
	Account(const std::string &name, float balance, int accountNo = -1);
	virtual ~Account() = default;
	[[nodiscard]] const std::string GetName()const;     // Encapsulation: read-only accessors for name
	[[nodiscard]] float GetBalance()const;
	[[nodiscard]] int GetAccountNo()const; // Encapsulation: read only accessor for account number
	[[nodiscard]] bool IsClosed() const;

	virtual void AccumulateInterest();
	virtual void Withdraw(float amount);
	void Deposit(float amount);
	[[nodiscard]] virtual float GetInterestRate()const;
	void Close();
	void Reopen();
	static void SyncAccountNumber(int lastIssued);
};
