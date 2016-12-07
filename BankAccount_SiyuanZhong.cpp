#include "BankAccount_SiyuanZhong.h"
	#include <iostream>
	#include <fstream>
	#include <string>
#include "myFunction_SiyuanZhong.h"
#include "Node_SiyuanZhong.h"
#include "StockAccount_SiyuanZhong.h"


using namespace std;

//Create an object use the data from BankHistory.txt
BankAccount::BankAccount(){
	ifstream ReadBankHistory;
	ReadBankHistory.open("BankHistory.txt");
	//if BankHistory.txt not exist, we create new one, and set CashBalence = 10000
	if (!ReadBankHistory.is_open()){
		ofstream newfile("BankHistory.txt");
		string Date = myDate();
		newfile << "Event\tAmount\t\tDate\tBalance\n"
			<< "Create\t$10000\t\t" << Date << "\t$10000";
		newfile.close();
		CashBalance = 10000.00;
		TotalPortfolio = CashBalance;
		ofstream newfile2("TotalPortfolio.txt");
		string Time = myTime();
		newfile2 << "Total Portfolio Value is $10000.00\t" << Time << '\t' << Date << endl;
		newfile2.close();
	}

	//if BankHistory.txt exist
	else
	{
		string value;
		while (!ReadBankHistory.eof()){
			char temp[200];
			ReadBankHistory.getline(temp, 200);
			value = temp;
		}
		int found = value.find_last_of('\t');
		string cash = value.substr(found + 2,value.length());
		CashBalance = atof(cash.c_str());
	}
	ReadBankHistory.close();
}

//Deposit function use to deposit money
void BankAccount::Deposit(double deposit){
	ofstream WriteBankHistory;
	CashBalance += deposit;
	WriteBankHistory.open("BankHistory.txt",ios::app);

	//All the information compose to one string
	char buffer[100];
	string Date = myDate();
	sprintf_s(buffer, "\r\nDeposit\t%.2f\t\t", deposit);
	string deposit_string = buffer;
	deposit_string.append(Date);
	sprintf_s(buffer, "\t$%.2f", CashBalance);
	deposit_string.append(buffer);
	
	//write to the txt file
	WriteBankHistory << deposit_string;

	//read the totalportfolio value from TotalPortfolio.txt
	ifstream TotalPortfolioFileIn;
	TotalPortfolioFileIn.open("TotalPortfolio.txt");
	if (TotalPortfolioFileIn.is_open())
	{
		string value;
		vector<string> allvalue;
		while (!TotalPortfolioFileIn.eof()){
			char temp[200];
			TotalPortfolioFileIn.getline(temp, 200);
			value = temp;
			allvalue.push_back(value);
		}
		int found1 = allvalue[ allvalue.size() - 2 ].find("$");
		int found2 = allvalue[allvalue.size() - 2].find('\t');
		string cash = allvalue[allvalue.size() - 2].substr(found1 + 1, found2);
		TotalPortfolio = atof(cash.c_str());
		TotalPortfolioFileIn.close();
	}
	
	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	TotalPortfolio += deposit;
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();
}

//withdrawal funcution use to withdrawal money
void BankAccount::Withdrawal(double withdraw){
	if (withdraw > CashBalance){
		cout << "This Bank Account don't have enough cash. Withdraw Fail.\n";
		return;
	}
	ofstream WriteBankHistory;
	CashBalance -= withdraw;
	WriteBankHistory.open("BankHistory.txt", ios::app);

	//All the information compose to one string
	char buffer[100];
	string Date = myDate();
	sprintf_s(buffer, "\r\nDraw\t%.2f\t\t", withdraw);
	string withdraw_string = buffer;
	withdraw_string.append(Date);
	sprintf_s(buffer, "\t$%.2f", CashBalance);
	withdraw_string.append(buffer);

	//write to the txt file
	WriteBankHistory <<	withdraw_string;

	//read the totalportfolio value from TotalPortfolio.txt
	ifstream TotalPortfolioFileIn;
	TotalPortfolioFileIn.open("TotalPortfolio.txt");
	if (TotalPortfolioFileIn.is_open())
	{
		string value;
		vector<string> allvalue;
		while (!TotalPortfolioFileIn.eof()){
			char temp[200];
			TotalPortfolioFileIn.getline(temp, 200);
			value = temp;
			allvalue.push_back(value);
		}
		int found1 = allvalue[allvalue.size() - 2].find("$");
		int found2 = allvalue[allvalue.size() - 2].find('\t');
		string cash = allvalue[allvalue.size() - 2].substr(found1 + 1, found2);
		TotalPortfolio = atof(cash.c_str());
		TotalPortfolioFileIn.close();
	}

	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	TotalPortfolio -= withdraw;
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();
}

//Print History
void BankAccount::PrintHistory(){
	//open file and output line by line
	ifstream History;
	History.open("BankHistory.txt",ios::in);
	if (!History){
		cout << "Error:cannot open file." << endl;
		return;
	}
	string value;
	char temp[100];
	while (!History.eof()){
		History.getline(temp, 100);
		value = temp;
		cout << value << endl;
	}
}

double BankAccount::getCashBalance(){
	return CashBalance;
}