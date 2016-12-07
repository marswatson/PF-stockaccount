#ifndef STOCKACCOUNT_SIYUANZHONG_H
#define STOCKACCOUNT_SIYUANZHONG_H

#include "Account_SiyuanZhong.h"
#include "BankAccount_SiyuanZhong.h"
#include "Node_SiyuanZhong.h"
#include <iostream>
#include <string.h>
#include <vector>

class StockAccount : public Account{
	friend class BankAccount;
public:
	//construct and destruct function;
	//when we construct we first need to judge if Portfolio.txt exist. If it is exis we ned to read data from it
	//and create new nodes to stores this data and use sort() function to sort according to current per price
	//Besides we also need to make sure if StockHistort.txt exist if not, we need to create new one
	StockAccount();
	~StockAccount();
	//Buy and Sell function, because buy and sell need cash,
	//so we also need a bank account to see if there are availible cash to buy or sell
	//Then open StockTranstionHistory.txt to write the trasaction history to this file
	//Next use StoreStockData(BankAccount*) function to store data to the BankHistory.txt
	//Finally change the TotalPortfolio.txt which record total portfolio value change
	void Buy(Node *, BankAccount*);
	void Sell(Node *, BankAccount*);
	//sort function, every time after we buy or sellwe need to sort the linkedlist.And when we display or initialize
	//stock account the current PricePerShare will change, we also need to use this function
	void sort();
	//if linked list is empty
	bool isEmpty() const;
	//get the random PricePerShare from Result1.txt and Result2.txt 
	//and store the data to a vector<double> Price and vector<string> StockName
	void GetPrice();
	//get current price per share according to the vector<double> Price and vector<string> StockName
	double GetPerPrice(string);
	//Every time we run program, and buy sell stock, deposit withdraw money, the account information will change
	//we need to store it into TotalPortfolio.txt 
	void StoreStockData(BankAccount*);
	//Display the TotalPortfolio.txt 
	void DisplayPotfolio(BankAccount*);
	//According to the current linked list and current price per share we get total value of stock account
	double GetTotalPrice();
	//when we buy or sell a stock we need to change bank account history 
	void SellStockBankRecord(Node*, BankAccount*);
	void BuyStockBankRecord(Node*, BankAccount*);
	//Display the Transaction History form StockTrastionHistory.txt file
	void DisplayTransactionHistory();
	//plot the total portfolio value change, it need to link with matlab. All the data read from TotalPortfolio.txt
	void PlotTotal(BankAccount*);
private:
	//head and tail node
	Node *header;
	Node *tail;
	//store current pricepershare
	vector<string> StockName;
	vector<double> Price;
};
#endif