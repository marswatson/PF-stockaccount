#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "Account_SiyuanZhong.h"
#include "Node_SiyuanZhong.h"
#include "StockAccount_SiyuanZhong.h"

using namespace std;

//define Bank Account information
class BankAccount : public Account{
	// StockAccount is friend class of BankAccount
	friend class StockAccount;
public:
	//BankAccount can support Deposit, Withdrawal and PrintHistory
	BankAccount();
	void Deposit(double);
	void Withdrawal(double);
	void PrintHistory();
	double getCashBalance();
private:
	double CashBalance;
};

#endif

