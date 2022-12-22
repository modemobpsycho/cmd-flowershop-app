#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include "Account.h"
using namespace std;

int menu_helper(vector <string>);
int input();
void returnTime(int& day, int& month, int& year);
float floatInput();
