// Name; Siyuan Zhong 168005338
// Course: 16:332:503:02, Programming Finance.
// Example: linked list
#ifndef NODE_SIYUANZHONG_H
#define NODE_SIYUANZHONG_H

#include <iostream>
#include <string.h>

using namespace std;

//node class it can be use to record item number and item name it also can point to next node
class Node{
public:
	string StockName;
	int StockNumber;
	double PricePerShare;
	double Total;
	Node *next;
	Node *previous;
	Node(int number, double price)
		: StockNumber(number), PricePerShare(price)
	{
		this->StockName.empty();
		this->next = NULL;
		this->previous = NULL;
	}
	Node(int number, double price, string name)
		: StockNumber(number), PricePerShare(price), StockName(name)
	{
		this->next = NULL;
		this->previous = NULL;
	}
};
#endif