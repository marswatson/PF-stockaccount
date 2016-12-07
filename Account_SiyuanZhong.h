#ifndef ACCOUNT_H
#define ACCOUNT_H

using namespace std;
#include <iostream>

//define Abstract Class
class Account{
public:
	double getTotalPortfolio(){
		return TotalPortfolio;
	};
protected:
	double TotalPortfolio;
};
#endif
