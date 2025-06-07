#include <iostream>
#include <iomanip>
using namespace std;

// Base Account class
class Account {
private:
	unsigned long long accountNumber;
	double balance;

public:
	// Constructor with default balance = 0.0
	Account(unsigned long long accNum, double initialBalance = 0.0)
		: accountNumber(accNum), balance(initialBalance) {
	}

	// Deposit method - adds amount to balance
	void deposit(double amount) {
		if (amount > 0) {
			balance += amount;
		}
	}

	// Virtual withdraw method - base implementation for standard accounts
	virtual bool withdraw(double amount) {
		if (amount > 0 && amount <= balance) {
			balance -= amount;
			return true;
		}
		return false;
	}

	// Getter for balance
	double getBalance() const {
		return balance;
	}

	// Getter for account number
	unsigned long long getAccountNumber() const {
		return accountNumber;
	}

	// Overload << operator for printing account details
	friend ostream& operator<<(ostream& os, const Account& account) {
		os << "Account #" << account.accountNumber
			<< " | Balance: $" << fixed << setprecision(2) << account.balance;
		return os;
	}

	// Virtual destructor for proper inheritance
	virtual ~Account() {}

protected:
	// Protected method to allow derived classes to modify balance
	void setBalance(double newBalance) {
		balance = newBalance;
	}
};

// BankAccount class inheriting from Account
class BankAccount : public Account {
private:
	double overdraftLimit;

public:
	// Constructor accepting account number, initial balance, and overdraft limit
	BankAccount(unsigned long long accNum, double initialBalance, double overdraft)
		: Account(accNum, initialBalance), overdraftLimit(overdraft) {
	}

	// Override withdraw method to allow overdraft
	bool withdraw(double amount) override {
		if (amount > 0 && amount <= (getBalance() + overdraftLimit)) {
			setBalance(getBalance() - amount);
			return true;
		}
		return false;
	}

	// Getter for overdraft limit
	double getOverdraftLimit() const {
		return overdraftLimit;
	}
};

int main() {
	// Create BankAccount object with specified parameters
	BankAccount account(10000001, 500.00, 200.00);

	// Display initial account details
	cout << "Account #" << account.getAccountNumber()
		<< " created with balance: $" << fixed << setprecision(2)
		<< account.getBalance() << endl;

	// Deposit $500.00
	cout << "Depositing $500.00..." << endl;
	account.deposit(500.00);
	cout << "Balance: $" << account.getBalance() << endl;

	// Attempt to withdraw $600.00 (should succeed)
	cout << "Withdrawing $600.00..." << endl;
	if (account.withdraw(600.00)) {
		cout << "Withdrawal successful. Balance: $" << account.getBalance() << endl;
	}
	else {
		cout << "Withdrawal failed. Balance: $" << account.getBalance() << endl;
	}

	// Attempt to withdraw $500.00 (should fail due to overdraft limit)
	cout << "Withdrawing $500.00..." << endl;
	if (account.withdraw(500.00)) {
		cout << "Withdrawal successful. Balance: $" << account.getBalance() << endl;
	}
	else {
		cout << "Withdrawal failed. Balance: $" << account.getBalance() << endl;
	}

	// Display final account details using overloaded << operator
	cout << "Final Account Details:" << endl;
	cout << account << endl;

	return 0;
}