#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <numeric> 
#include "Account_SiyuanZhong.h"
#include "BankAccount_SiyuanZhong.h"
#include "StockAccount_SiyuanZhong.h"

using std::cout;
using std::cin;
using std::endl;
using namespace std;

int main(void)
{
	//create two object
	StockAccount stock;
	BankAccount bank;
	int main_choice = 0;
	string symbol;
	int number;
	double price,current;
	cout << "Welcome to the Account Management System.\n";
	while (main_choice != 3){
		cout << "Please select an account to access:\n"
			<< "1. Stock Portfolio Account\n"
			<< "2. Bank Account\n"
			<< "3. Exit\n"
			<< "Option : ";
		cin >> main_choice;
		if (main_choice == 1){
			int choice = 0;
			cout << "Stock Portfolio Account";
			while (choice != 7)
			{
				cout << "Please select an option:\n"
					<< "1. Display the price for a stock symbol\n"
					<< "2. Display the current portfolio\n"
					<< "3. Buy shares\n"
					<< "4. Sell shares\n"
					<< "5. View a graph for the portfolio value\n"
					<< "6. View transaction history\n"
					<< "7. Return to previous menu\n"
					<< "Option:";
				cin >> choice;
				switch (choice)
				{
				case 1:{
					cout << "Please enter the stock symbol:";
					cin >> symbol;
					price = stock.GetPerPrice(symbol);
					if (price == 0)
						break;
					cout << "Company Symbol Price per share\n"
						<< symbol << "$" << price << endl;
					break;
				}
				case 2:{
					stock.DisplayPotfolio(&bank);
					break;
				}
				case 3:{
					cout << "Please enter the stock symbol you wish to purchase :";
					cin >> symbol;
					cout << "Please enter the number of shares : ";
					cin >> number;
					cout << "Please enter the maximum amount you are willing to pay per share : ";
					cin >> price;
					current = stock.GetPerPrice(symbol);
					if (current == 0){
						break;
					}
					if (price < current)
					{
						cout << "Price per share is greater than the maximum. Purchase fail.\n";
						break;
					}
					Node *node = new Node(number, stock.GetPerPrice(symbol), symbol);
					stock.Buy(node, &bank);
					break;
				}
				case 4:{
					cout << "Please enter the stock symbol you wish to sell :";
					cin >> symbol;
					cout << "Please enter the number of shares : ";
					cin >> number;
					cout << "Please enter the minimum amount you are willing to sell per share : ";
					cin >> price;
					current = stock.GetPerPrice(symbol);
					if (current == 0){
						break;
					}
					if (price > current){
						cout << "Price per share is less than the minimum. Sell fail.\n";
						break;
					}
					Node *node = new Node(number, stock.GetPerPrice(symbol), symbol);
					stock.Sell(node, &bank);
					break;
				}
				case 5:{
					stock.PlotTotal(&bank);
					break;
				}
				case 6:{
					stock.DisplayTransactionHistory();
					break;
				}
				case 7:{
					break;
				}
				default:{
					cout << "This option is not supported.\n";
					break;
				}
				}
				if (choice == 7)
					break;
			}
			continue;
		}
		if (main_choice == 2){
			int choice = 0;
			cout << "Bank Account";
			while (choice != 5)
			{
				cout << "Please select an option:\n"
					<< "1. View account balance\n"
					<< "2. Deposit money\n"
					<< "3. Withdraw money\n"
					<< "4. Print out history\n"
					<< "5. Return to previous menu\n"
					<< "Option:";
				cin >> choice;
				switch (choice)
				{
				case 1:
					cout << "You have $" << bank.getCashBalance() << " in your bank account.\n";
					break;
				case 2:
					cout << "Please select the amount you wish to deposit: $";
					cin >> price;
					bank.Deposit(price);
					stock.StoreStockData(&bank);
					break;
				case 3:
					cout << "Please select the amount you wish to withdraw: $";
					cin >> price;
					bank.Withdrawal(price);
					stock.StoreStockData(&bank);
					break;
				case 4:
					bank.PrintHistory();
					break;
				case 5:
					break;
				default:
					break;
				}
			}
			continue;
		}
		if (main_choice == 3){
			break;
		}
		cout << "This option is not support.\n";
		continue;
	}
	return 0;
}

