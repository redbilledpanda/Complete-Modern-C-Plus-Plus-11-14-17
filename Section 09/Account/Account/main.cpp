#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "Checking.h"
#include "Savings.h"

namespace {
	struct PersistedAccount {
		std::string type;
		std::string name;
		float balance{};
		float extra{};			// Minimum balance for checking or rate for savings
		int accountNo{};
		bool closed{};
	};

	struct PersistResult {
		std::vector<PersistedAccount> accounts;
		int lastIssued{};
	};

	std::string Trim(const std::string &value) {
		const auto start = value.find_first_not_of(" \t");
		if (start == std::string::npos) return {};
		const auto end = value.find_last_not_of(" \t");
		return value.substr(start, end - start + 1);
	}

	std::string ToLower(std::string text) {
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return text;
	}

	bool EqualsIgnoreCase(const std::string &lhs, const std::string &rhs) {
		return ToLower(lhs) == ToLower(rhs);
	}

	PersistResult LoadPersistedAccounts(const std::string &path) {
		std::ifstream in(path);
		if (!in) {
			return {};
		}
		PersistResult result{};
		PersistedAccount current{};
		bool hasData = false;
		std::string line;
		while (std::getline(in, line)) {
			const auto trimmed = Trim(line);
			if (trimmed.empty()) continue;
			if (trimmed == "---") {
				if (!current.name.empty()) {
					result.accounts.push_back(current);
					result.lastIssued = std::max(result.lastIssued, current.accountNo);
				}
				current = {};
				hasData = false;
				continue;
			}
			const auto colon = trimmed.find(':');
			if (colon == std::string::npos) continue;
			const auto key = ToLower(Trim(trimmed.substr(0, colon)));
			const auto value = Trim(trimmed.substr(colon + 1));
			if (key == "generator") {
				result.lastIssued = std::max(result.lastIssued, std::stoi(value));
				continue;
			}
			hasData = true;
			if (key == "type") current.type = ToLower(value);
			else if (key == "name") current.name = value;
			else if (key == "balance") current.balance = std::stof(value);
			else if (key == "extra") current.extra = std::stof(value);
			else if (key == "accountno") current.accountNo = std::stoi(value);
			else if (key == "closed") current.closed = (value == "1" || ToLower(value) == "true");
		}
		if (hasData && !current.name.empty()) {
			result.accounts.push_back(current);
			result.lastIssued = std::max(result.lastIssued, current.accountNo);
		}
		return result;
	}

	std::unique_ptr<Account> CreateAccountFromData(const PersistedAccount &data) {
		if (data.type == "checking") {
			auto acc = std::make_unique<Checking>(data.name, data.balance, data.extra, data.accountNo);
			if (data.closed) acc->Close();
			return acc;
		}
		if (data.type == "savings") {
			auto acc = std::make_unique<Savings>(data.name, data.balance, data.extra, data.accountNo);
			if (data.closed) acc->Close();
			return acc;
		}
		return nullptr;
	}

	void SaveAccounts(const std::vector<std::unique_ptr<Account>> &accounts, const std::string &path) {
		std::ofstream out(path, std::ios::trunc);
		if (!out) {
			std::cerr << "Failed to persist account data to " << path << '\n';
			return;
		}
		int maxAccNo = 0;
		for (const auto &account : accounts) {
			maxAccNo = std::max(maxAccNo, account->GetAccountNo());
		}
		out << "generator:" << maxAccNo << '\n';
		for (const auto &account : accounts) {
			if (auto *checking = dynamic_cast<const Checking *>(account.get())) {
				out << "type:checking\n";
				out << "name:" << account->GetName() << '\n';
				out << "balance:" << account->GetBalance() << '\n';
				out << "extra:" << checking->GetMinimumBalance() << '\n';
			}
			else if (auto *savings = dynamic_cast<const Savings *>(account.get())) {
				out << "type:savings\n";
				out << "name:" << account->GetName() << '\n';
				out << "balance:" << account->GetBalance() << '\n';
				out << "extra:" << savings->GetInterestRate() << '\n';
			}
			else {
				out << "type:account\n";
				out << "name:" << account->GetName() << '\n';
				out << "balance:" << account->GetBalance() << '\n';
				out << "extra:0\n";
			}
			out << "accountNo:" << account->GetAccountNo() << '\n';
			out << "closed:" << (account->IsClosed() ? 1 : 0) << '\n';
			out << "---\n";
		}
	}

	std::string ReadLine(const std::string &message) {
		std::cout << message;
		std::string line;
		std::getline(std::cin, line);
		return line;
	}

	int PromptInt(const std::string &message) {
		while (true) {
			std::string line = ReadLine(message);
			std::stringstream ss(line);
			int value{};
			if (ss >> value) {
				return value;
			}
			std::cout << "Please enter a valid number.\n";
		}
	}

	float PromptAmount(const std::string &message) {
		while (true) {
			std::string line = ReadLine(message);
			std::stringstream ss(line);
			float amount{};
			if (ss >> amount) {
				return amount;
			}
			std::cout << "Enter a numeric amount.\n";
		}
	}

	bool NameExists(const std::vector<std::unique_ptr<Account>> &accounts, const std::string &name) {
		return std::any_of(accounts.begin(), accounts.end(), [&](const auto &acc) { return EqualsIgnoreCase(acc->GetName(), name); });
	}

	std::string AccountType(const Account &account) {
		if (dynamic_cast<const Checking *>(&account) != nullptr) return "checking";
		if (dynamic_cast<const Savings *>(&account) != nullptr) return "savings";
		return "account";
	}

	std::string AccountLabel(const Account &account) {
		std::string label = account.GetName();
		if (account.IsClosed()) {
			label += " (closed)";
		}
		return label;
	}

	void PrintDetails(const Account &account) {
		std::cout << "\n=== Account Details ===\n";
		std::cout << "Name: " << account.GetName() << '\n';
		std::cout << "Account #: " << account.GetAccountNo() << '\n';
		std::cout << "Type: " << AccountType(account) << '\n';
		std::cout << "Balance: " << account.GetBalance() << '\n';
		std::cout << "Status: " << (account.IsClosed() ? "closed" : "open") << '\n';

		if (auto *checking = dynamic_cast<const Checking *>(&account)) {
			std::cout << "Minimum balance requirement: " << checking->GetMinimumBalance() << '\n';
		}
		else if (auto *savings = dynamic_cast<const Savings *>(&account)) {
			std::cout << "Interest rate: " << savings->GetInterestRate() << '\n';
		}
		std::cout << "=======================\n";
	}

	std::unique_ptr<Account> PromptForNewAccount(const std::vector<std::unique_ptr<Account>> &accounts) {
		std::string name;
		while (true) {
			name = ReadLine("Enter account holder name (unique): ");
			if (!NameExists(accounts, name)) break;
			std::cout << "Name already exists. Please choose another.\n";
		}

		const float startingBalance = PromptAmount("Enter starting balance: ");

		std::string typeChoice;
		typeChoice = ReadLine("Account type (checking/savings): ");
		typeChoice = ToLower(typeChoice);

		if (typeChoice == "checking") {
			const float minBalance = PromptAmount("Enter minimum balance requirement: ");
			return std::make_unique<Checking>(name, startingBalance, minBalance);
		}
		else if (typeChoice == "savings") {
			const float rate = PromptAmount("Enter interest rate (e.g. 0.05 for 5%): ");
			return std::make_unique<Savings>(name, startingBalance, rate);
		}
		else {
			std::cout << "Unknown type. Defaulting to savings account with 0% rate.\n";
			return std::make_unique<Savings>(name, startingBalance, 0.0f);
		}
	}

	void PrintSummary(const Account &account) {
		std::cout << "\nAccount #" << account.GetAccountNo() << " (" << account.GetName() << ")\n";
		std::cout << "Balance: " << account.GetBalance() << '\n';
		std::cout << "Closed: " << (account.IsClosed() ? "yes" : "no") << '\n';
	}
}

int main() {
	const std::string kAccountFile = "accounts_store.txt";
	auto persisted = LoadPersistedAccounts(kAccountFile);
	Account::SyncAccountNumber(persisted.lastIssued);

	std::vector<std::unique_ptr<Account>> accounts;
	accounts.reserve(5);
	for (const auto &data : persisted.accounts) {
		if (accounts.size() >= 5) break;
		if (NameExists(accounts, data.name)) continue; // enforce uniqueness
		if (auto acc = CreateAccountFromData(data)) {
			accounts.push_back(std::move(acc));
		}
	}

	std::cout << "Existing accounts:\n";
	if (accounts.empty()) {
		std::cout << "None\n";
	}
	else {
		for (const auto &acc : accounts) {
			std::cout << " - " << AccountLabel(*acc) << '\n';
		}
	}

	int selectedIndex = -1;
	if (!accounts.empty()) {
		std::cout << "Enter the name of the account to load (or press Enter to create a new one): ";
		std::string choice;
		std::getline(std::cin, choice);
		if (!choice.empty()) {
			for (size_t i = 0; i < accounts.size(); ++i) {
				if (EqualsIgnoreCase(accounts[i]->GetName(), choice)) {
					selectedIndex = static_cast<int>(i);
					break;
				}
			}
			if (selectedIndex == -1) {
				std::cout << "No matching account found; creating a new one.\n";
			}
		}
	}

	if (selectedIndex == -1) {
		if (accounts.size() >= 5) {
			std::cout << "Maximum of 5 accounts reached. Cannot create a new account.\n";
			return 0;
		}
		accounts.push_back(PromptForNewAccount(accounts));
		selectedIndex = static_cast<int>(accounts.size() - 1);
	}

	bool running = true;
	while (running) {
		Account &account = *accounts[selectedIndex];

		std::cout << "\nChoose an action:\n";
		std::cout << "1) Show account status\n2) Deposit\n3) Withdraw\n4) Add interest\n5) Close account\n6) Re-open account\n7) Switch account\n8) Save and exit\n9) Exit without saving\n";
		int option = PromptInt("Selection: ");
		switch (option) {
		case 1:
			try {
				PrintDetails(account);
			}
			catch (const std::exception &ex) {
				std::cout << "Unable to read account: " << ex.what() << '\n';
			}
			break;
		case 2: {
			const float amount = PromptAmount("Deposit amount: ");
			try {
				account.Deposit(amount);
			}
			catch (const std::exception &ex) {
				std::cout << "Deposit failed: " << ex.what() << '\n';
			}
			break;
		}
		case 3: {
			const float amount = PromptAmount("Withdraw amount: ");
			try {
				account.Withdraw(amount);
			}
			catch (const std::exception &ex) {
				std::cout << "Withdrawal failed: " << ex.what() << '\n';
			}
			break;
		}
		case 4:
			try {
				account.AccumulateInterest();
			}
			catch (const std::exception &ex) {
				std::cout << "Interest calculation failed: " << ex.what() << '\n';
			}
			break;
		case 5:
			account.Close();
			std::cout << "Account closed. Further transactions will be blocked.\n";
			break;
		case 6:
			if (!account.IsClosed()) {
				std::cout << "Account is already open.\n";
				break;
			}
			if (account.GetBalance() <= 0) {
				std::cout << "Cannot re-open an empty account. Deposit funds required.\n";
				break;
			}
			account.Reopen();
			std::cout << "Account re-opened.\n";
			break;
		case 7: {
			if (accounts.empty()) {
				std::cout << "No accounts available to switch.\n";
				break;
			}
			std::cout << "Available accounts:\n";
			for (const auto &acc : accounts) {
				std::cout << " - " << AccountLabel(*acc) << '\n';
			}
			std::cout << "Enter name to switch, or leave empty to create new: ";
			std::string name;
			std::getline(std::cin, name);
			if (name.empty()) {
				if (accounts.size() >= 5) {
					std::cout << "Cannot create more than 5 accounts.\n";
					break;
				}
				accounts.push_back(PromptForNewAccount(accounts));
				selectedIndex = static_cast<int>(accounts.size() - 1);
				break;
			}
			bool found = false;
			for (size_t i = 0; i < accounts.size(); ++i) {
				if (EqualsIgnoreCase(accounts[i]->GetName(), name)) {
					selectedIndex = static_cast<int>(i);
					found = true;
					break;
				}
			}
			if (!found) {
				std::cout << "No account with that name.\n";
			}
			break;
		}
		case 8:
			SaveAccounts(accounts, kAccountFile);
			running = false;
			break;
		case 9:
			running = false;
			break;
		default:
			std::cout << "Unknown option. Try again.\n";
			break;
		}
	}

	std::cout << "Goodbye!\n";
	return 0;
}
