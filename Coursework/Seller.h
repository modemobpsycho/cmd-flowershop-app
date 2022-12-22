#pragma once
#include <map>
#include <string>
#include "Account.h"
#include "Menu.h"
using namespace std;

void menu_seller(map <string, FlowerAdmin>& flowers);
void updateFileChecks(vector <Check>& check, vector<Check>& checksFull);
