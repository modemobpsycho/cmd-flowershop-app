#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include "Flower.h"
using namespace std;

class Order
{
protected:
	vector <FlowerOrder> cart;
	float summaFull, saleFull;
	int ID;
public:
	Order();
	friend ofstream& operator<<(ofstream& out, const Order& order);
	friend ifstream& operator>>(ifstream& in, Order& order);
	void updateFlag(map <string, FlowerAdmin>& flowers);
	friend void workCart(Order& cart, int ID);
	friend void myOrders(User& user);
	friend void topUpShoppingCart(map <string, FlowerAdmin>& flowers, User& user);
	friend void deleteCart(User& user);
};

class Check : public Order
{
	int day, month, year;
	bool purchase;
public:
	Check();
	Check(vector <FlowerOrder> cart, float summaFull, float saleFull, int ID, bool purchase);
	friend ofstream& operator<<(ofstream& out, const Check& check);
	friend ifstream& operator>>(ifstream& in, Check& check);
	bool returnPurchase();
	int returnID();
	friend void checkOut(Check& check, int number);
	friend Check placeOrder(shared_ptr <Check> ptr, map <string, FlowerAdmin>& flowers);
	friend void Stat();
};

