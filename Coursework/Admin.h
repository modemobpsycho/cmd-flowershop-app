#pragma once
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include "Account.h"
#include "Menu.h"
#include "Order.h"
using namespace std;

void menu_admin(map <string, Account>& accounts, map <string, User>& users, map <string, FlowerAdmin>& flowers, string login);
void menu_accountsManagement(map <string, Account>& accounts, string login);
void menu_usersManagement(map<string, User>& users);
void menu_flowerManagement(map <string, FlowerAdmin>& flowers);
void addFlower(map <string, FlowerAdmin>& flowers);