#include "StockAccount_SiyuanZhong.h"
#include "Node_SiyuanZhong.h"
#include "BankAccount_SiyuanZhong.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <numeric> 
#include<cmath>
#include "myFunction_SiyuanZhong.h"

#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmex.lib" )  
#pragma comment( lib, "libeng.lib" )

#include <yvals.h>
#if (_MSC_VER >= 1600)
#define __STDC_UTF_16__
#endif
#include "mex.h"
// end

#include<engine.h>

using namespace std;

//counstruct LinkedList
StockAccount::StockAccount(){
	GetPrice();
	header = new Node(NULL, NULL);
	tail = new Node(NULL, NULL);
	header -> next = tail;
	tail -> previous = header;

	//we read the data from the Portfolio.txt
	ifstream ReadPortfolio;
	vector<string> LastStockAccountData;
	vector<int> LastStockAccountNumber;
	vector<string> LastStockAccountName;
	ReadPortfolio.open("Portfolio.txt");
	if (ReadPortfolio.is_open()){
		string value;
		char temp[100];
		while (!ReadPortfolio.eof()){
			ReadPortfolio.getline(temp, 100);
			value = temp;
			LastStockAccountData.push_back(value);
		}
		for (int i = 3; i < LastStockAccountData.size() - 1; i++){
			//extract name from file and store it into a vector
			int found1 = LastStockAccountData[i].find_first_of('\t', 0);
			string temp = LastStockAccountData[i].substr(0, found1);
			LastStockAccountName.push_back(temp);

			//extract number from file and store it into a vector
			int found2 = LastStockAccountData[i].find('\t',found1+1);
			temp = LastStockAccountData[i].substr(found1, found2);
			LastStockAccountNumber.push_back( atof( temp.c_str() ) );
		}

		//According to the Portfolio.txt create a Linked List
		for (int i = 0; i < LastStockAccountName.size(); i++){
			double perprice = GetPerPrice(LastStockAccountName[i]);
			Node *node = new Node(LastStockAccountNumber[i], perprice, LastStockAccountName[i]);
			Node *current;
			current = header->next;
			while (current != tail){
				current = current->next;
			}
			//new node at the last
			if (current == tail){
				current->previous->next = node;
				node->previous = tail->previous;
				node->next = tail;
				tail->previous = node;
			}
		}
		sort();
	}

	//if StockHistory.txt not exist, we create new one
	ifstream ReadStockHistory;
	ReadStockHistory.open("StockTranstionHistory.txt");
	if (!ReadStockHistory.is_open()){
		cout << " We will create new StockTranstionHistory file. \n" << endl;
		ofstream newfile("StockTranstionHistory.txt");
		newfile << "Event\tCompSym\tNumber\tSharePrice\tTotal\ttime";
		newfile.close();
	}

}

//destruct LinkedList()
StockAccount::~StockAccount(){

	if (!isEmpty()) {    // List is not empty
		cout << "Destroying nodes ...\n";
		Node *currentPtr = header;
		Node *tempPtr;
		while (currentPtr != 0) {  // delete remaining nodes
			tempPtr = currentPtr;
			currentPtr = currentPtr->next;
			delete tempPtr;
		} // end while
	}
}

//if LinkedList is empty
bool StockAccount::isEmpty() const{
	return header -> next == tail;
}

//Buy Stock
void StockAccount::Buy(Node *node,BankAccount *bankaccount){
	GetPrice();
	//make sure bank account have enough balance
	if (bankaccount->CashBalance < node->StockNumber * GetPerPrice(node->StockName) ){
		cout << "Bank Account don't have enough balance. Purchase Fail.\n";
		return;
	}
	cout << "You have purchased " << node->StockNumber 
		<< " shares of "<< node->StockName <<" at $" << GetPerPrice(node->StockName)
		<< " each for a total of\n$"
		<< node->StockNumber * GetPerPrice(node->StockName) << endl;

	//All the information compose to one string and output to the StockHistory
	ofstream WriteBankHistory;
	WriteBankHistory.open("StockTranstionHistory.txt", ios::app);
	char buffer[100];
	string time = myTime();
	sprintf_s(buffer, "\r\nBuy\t");
	string stock_string = buffer;
	stock_string.append(node->StockName);
	sprintf_s(buffer, 
		"\t%d\t%.2f\t\t%.2f\t", node->StockNumber, node->PricePerShare, node->StockNumber*node->PricePerShare);
	stock_string.append(buffer);
	stock_string.append(time);
	WriteBankHistory << stock_string;

	//change the bank account history and cash balance
	BuyStockBankRecord(node, bankaccount);

	//if double linked list is empty
	if (isEmpty()){
		header->next = node;
		node->previous = header;
		node->next = tail;
		tail->previous = node;
		StoreStockData(bankaccount);

		//Store the information to the TotalPortfolio.txt which record all the money information
		ofstream TotalPortfolioFile;
		TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
		string total_string = "Total Portfolio Value is $";
		TotalPortfolio = GetTotalPrice() + bankaccount->CashBalance;
		sprintf_s(buffer, "%.2f\t", TotalPortfolio);
		total_string.append(buffer);
		string Date = myDate();
		string Time = myTime();
		TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
		TotalPortfolioFile.close();
		sort();
		return;
	}

	//if is not empty traverse all nodes
	Node *current;
	current = header->next;
	while (current != tail){
		if (node->StockName == current->StockName){
			current->StockNumber += node->StockNumber;
			break;
		}
		current = current->next;
	}
	if (current == tail){
		current->previous->next = node;
		node->previous = tail->previous;
		node->next = tail;
		tail->previous = node;
	}
	StoreStockData(bankaccount);

	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	TotalPortfolio = GetTotalPrice() + bankaccount->CashBalance;
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Date = myDate();
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();
	sort();
}

//Sell Stock
void StockAccount::Sell(Node *node,BankAccount *bankaccount){
	GetPrice();
	if (isEmpty()){
		cout << "This stock doesn't exist." << endl;
		return;
	}

	//if is not empty traverse all nodes make sure the stock is in the LinkedList
	Node *current;
	current = header->next;
	while (current != tail){
		//if find the stock have same name then operate
		if (node->StockName == current->StockName)
		{
			current->StockNumber -= node->StockNumber;
			if (current->StockNumber < 0)
			{
				cout << "The Stock Number can not less than 0. We will cancle this option." << endl;
				current->StockNumber += node->StockNumber;
				return;
			}

			//If the exist StockNumber >= 0
			cout << "You have sell" << node->StockNumber << "shares of " 
				<< node->StockName << " at $" << GetPerPrice(node->StockName)
				<< " each for a total of\n$"
				<< node->StockNumber * GetPerPrice(node->StockName) << endl;

			//All the information compose to one string and output to the StockHistory
			ofstream WriteBankHistory;
			WriteBankHistory.open("StockTranstionHistory.txt", ios::app);
			char buffer[100];
			string time = myTime();
			sprintf_s(buffer, "\r\nSell\t");
			string stock_string = buffer;
			stock_string.append(node->StockName);
			sprintf_s(buffer,"\t%d\t%.2f\t\t%.2f\t", node->StockNumber, node->PricePerShare, node->StockNumber*node->PricePerShare);
			stock_string.append(buffer);
			stock_string.append(time);
			WriteBankHistory << stock_string;

			//change the bank account history and cash balance
			SellStockBankRecord(node, bankaccount);

			if (current->StockNumber == 0){
				Node *temp = current;
				current = temp->previous;
				current->next = temp->next;
				temp->next->previous = current;
				delete temp;
			}
			break;
		}
		current = current->next;
	}
	if (current == tail){
		cout << "This stock doesn't exist." << endl;
		return;
	}
	StoreStockData(bankaccount);

	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	TotalPortfolio = GetTotalPrice() + bankaccount->CashBalance;
	char buffer[100];
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Date = myDate();
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();
	sort();
}

//Display the current Price
void StockAccount::GetPrice(){
	Price.clear();
	StockName.clear();
	//load the two results file include NAME and PRICE
	ifstream Result1,Result2;
	char temp[100];
	string value;
	vector<string> buffer1;
	vector<string> buffer2;
	vector<double> Price1;
	vector<double> Price2;
	Result1.open("Results_1.txt", ios::in);
	Result2.open("Results_2.txt", ios::in);
	if (!Result1||!Result2){
		cout << "Error:cannot open file." << endl;
		return;
	}
	while (!Result1.eof()){
		Result1.getline(temp, 100);
		value = temp;
		buffer1.push_back(value);
	}
	while (!Result2.eof()){
		Result2.getline(temp, 100);
		value = temp;
		buffer2.push_back(value);
	}
	Result1.close();
	Result2.close();

	//allocate NAME and PRICE to a vector space
	for (int i = 0; i < buffer1.size(); i++){
		int found1 = buffer1[i].find_first_of('\t', 0);
		string temp = buffer1[i].substr(0,found1);
		StockName.push_back(temp);

		int found2 = buffer1[i].find_last_of('\t', 0);
		temp = buffer1[i].substr(found1 + 1, found2);
		Price1.push_back( atof( temp.c_str() ) );

		int found3 = buffer2[i].find_last_of('\t', 0);
		temp = buffer2[i].substr(found1 + 1, found3);
		Price2.push_back( atof( temp.c_str() ) );
	}

	//Random choose Price from two result
	srand(time(0));
	int random_number;
	for (int i = 0; i < Price1.size(); i++){
		random_number = rand() % 2 + 1;
		if (random_number == 1)
			Price.push_back(Price1[i]);
		else
			Price.push_back(Price2[i]);
	}
}

//Sort function to sort the linked list
void StockAccount::sort(){
	double max;
	double currentprice;
	int size = 0;
	if (isEmpty())
		return;

	//use bubble sort to let the minum total price at the last
	Node *current = header->next;
	while (current != tail->previous){
		max = current->StockNumber * GetPerPrice(current->StockName);
		currentprice = current->next->StockNumber * GetPerPrice(current->next->StockName);
		if (currentprice > max){
			Node *temp = current->next;
			current->previous->next = temp;
			temp->previous = current->previous;
			current->next = temp->next;
			current->previous = temp;
			temp->next = current;
			current->next->previous = current;
			size++;
			continue;
		}
		current = current->next;
	}
	//once we get size we can loop another size times
	for (int i = 0; i < size; i++){
		current = header->next;
		while (current != tail->previous){
			max = current->StockNumber * GetPerPrice(current->StockName);
			currentprice = current->next->StockNumber * GetPerPrice(current->next->StockName);
			if (currentprice > max){
				Node *temp = current->next;
				current->previous->next = temp;
				temp->previous = current->previous;
				current->next = temp->next;
				current->previous = temp;
				temp->next = current;
				current->next->previous = current;
				size++;
				continue;
			}
			current = current->next;
		}
	}
}

//Get the current price frome vector
double StockAccount::GetPerPrice(string stockname){
	vector< string >::iterator location;
	location = find(StockName.begin(), StockName.end(), stockname);
	int position = location - StockName.begin();
	if (position < 0 || position > StockName.size() - 1){
		cout << "This symbol is not exist.\n";
		return 0;
	}
	return Price[position];
}

//get total price from every node
double StockAccount::GetTotalPrice(){
	GetPrice();
	sort();
	double total = 0;
	Node *current = header->next;
	while (current != tail)
	{
		total += current->StockNumber * GetPerPrice(current->StockName);
		current = current->next;
	}
	return total;
}

//Every time we finish operation we need to store the data into Potfolio.txt
void StockAccount::StoreStockData(BankAccount *bank){
	//In this part we first sort Double linked list then we input the information to the Portfolio.txt
	GetPrice();
	sort();
	ofstream WritePortfolio;
	WritePortfolio.open("Portfolio.txt", ios::out);
	char buffer[100];
	string Sentence;

	//input first line cash
	Sentence = "Cash Balance = $";
	sprintf_s(buffer, "%.2f", bank->CashBalance);
	Sentence.append(buffer);
	WritePortfolio << Sentence;

	//input second line total portfolio
	TotalPortfolio = bank->CashBalance + GetTotalPrice();
	Sentence = "\r\nTotal portfolio value: $";
	sprintf_s(buffer, "%.2f", TotalPortfolio);
	Sentence.append(buffer);
	WritePortfolio << Sentence;

	//input the stock information
	Sentence = "\r\nCompSym\tNumber\tPrice\tTotal\r\n";
	WritePortfolio << Sentence;
	Node *current = header->next;
	while (current != tail){
		Sentence = current->StockName;
		sprintf_s(buffer, "\t%d\t", current->StockNumber);
		Sentence.append(buffer);
		sprintf_s(buffer, "%.2f\t%.2f\r\n",
			GetPerPrice(current->StockName), GetPerPrice(current->StockName)*current->StockNumber);
		Sentence.append(buffer);
		WritePortfolio << Sentence;
		current = current->next;
	}
	WritePortfolio.close();
}

//Display the Portfolio
void StockAccount::DisplayPotfolio(BankAccount *bank){
	//First we check if all the node is store to Portfolio
	GetPrice();
	sort();
	StoreStockData(bank);
	
	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	char buffer[100];
	TotalPortfolio = GetTotalPrice() + bank->CashBalance;
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Date = myDate();
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();

	//print the Portfolio in the screen
	ifstream History;
	History.open("Portfolio.txt", ios::in);
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

//Store Buy Stock Record
void StockAccount::BuyStockBankRecord(Node *node, BankAccount *bankaccount){
	ofstream WriteBankHistory;
	WriteBankHistory.open("BankHistory.txt", ios::app);
	bankaccount->CashBalance = bankaccount->CashBalance - node->StockNumber * GetPerPrice(node->StockName);
	//All the information compose to one string
	char buffer[100];
	string Date = myDate();
	sprintf_s(buffer, "\r\nBuy\t%.2f\t\t", node->StockNumber * GetPerPrice(node->StockName));
	string withdraw_string = buffer;
	withdraw_string.append(Date);
	sprintf_s(buffer, "\t$%.2f", bankaccount->CashBalance);
	withdraw_string.append(buffer);

	//write to the txt file
	WriteBankHistory << withdraw_string;
}

//Store Buy Stock Record
void StockAccount::SellStockBankRecord(Node *node, BankAccount *bankaccount){
	ofstream WriteBankHistory;
	WriteBankHistory.open("BankHistory.txt", ios::app);
	bankaccount->CashBalance = bankaccount->CashBalance + node->StockNumber * GetPerPrice(node->StockName);
	//All the information compose to one string
	char buffer[100];
	string Date = myDate();
	sprintf_s(buffer, "\r\nSell\t%.2f\t\t", node->StockNumber * GetPerPrice(node->StockName));
	string withdraw_string = buffer;
	withdraw_string.append(Date);
	sprintf_s(buffer, "\t$%.2f", bankaccount->CashBalance);
	withdraw_string.append(buffer);

	//write to the txt file
	WriteBankHistory << withdraw_string;
}

//Display the trasaction history
void StockAccount::DisplayTransactionHistory(){
	ifstream History;
	History.open("StockTranstionHistory.txt", ios::in);
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

//Use matlab to plot the total value change
void StockAccount::PlotTotal(BankAccount *bank){
	//get the current total data
	//Store the information to the TotalPortfolio.txt which record all the money information
	ofstream TotalPortfolioFile;
	TotalPortfolioFile.open("TotalPortfolio.txt", ios::app);
	string total_string = "Total Portfolio Value is $";
	char buffer[100];
	TotalPortfolio = GetTotalPrice() + bank->CashBalance;
	sprintf_s(buffer, "%.2f\t", TotalPortfolio);
	total_string.append(buffer);
	string Date = myDate();
	string Time = myTime();
	TotalPortfolioFile << total_string << Time << '\t' << Date << endl;
	TotalPortfolioFile.close();

	//use vector store data
	vector<string> TotalData;
	vector<double> TotalPrice;
	//read the file
	ifstream TotalPortfolioFileIn;
	TotalPortfolioFileIn.open("TotalPortfolio.txt", ios::in);
	if (!TotalPortfolioFileIn){
		cout << "Error:cannot open file." << endl;
		return;
	}
	string value;
	char temp[100];
	while (!TotalPortfolioFileIn.eof()){
		TotalPortfolioFileIn.getline(temp, 100);
		value = temp;
		TotalData.push_back(value);
	}
	for (int i = 0; i < TotalData.size() - 1; i++){
		int found1 = TotalData[i].find("$");
		int found2 = TotalData[i].find('\t');
		value = TotalData[i].substr(found1 + 1, found2);
		TotalPrice.push_back( atof(value.c_str() ) );
	}

	// creating a pointer to the engine
	Engine *ep;
	ep = engOpen(NULL);
	if (ep == NULL)
	{
		cout << "Error: Not Found." << endl;
		exit(1);
	}
	// end

	mxArray *A;
	int n = TotalPrice.size();
	double d[100];
	for (int i = 0; i < n; i++){
		d[i] = TotalPrice[i];
	}
	A = mxCreateDoubleMatrix(1, n, mxREAL);
	memcpy((void *)mxGetPr(A), (void *)d, n*sizeof(double));
	engPutVariable(ep, "x", A);


	engEvalString(ep, "X=x;");
	engEvalString(ep, "plot(X);");	//call plot function


	engEvalString(ep, "title('Total Portfolio Change');");

	mxDestroyArray(A);

}
