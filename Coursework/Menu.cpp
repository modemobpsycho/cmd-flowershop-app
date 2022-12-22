#include "Menu.h"

int menu_helper(vector <string> menu)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int choice = 1;
	while (true)
	{
		system("cls");
		cout << menu[0];
		cout.setf(ios::right);
		cout.width(120 - menu[0].size());
		cout << "ESC - для выхода в предыдущее меню" << endl << endl;
		cout.unsetf(ios::right);
		for (int i = 1; i < menu.size(); i++)
		{
			if (i == choice)
			{
				SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				cout << menu[i] << endl;
				SetConsoleTextAttribute(hConsole, 15);
				continue;
			}
			cout << menu[i] << endl;
		}

		char c = _getch();
		if (c == 72)
		{
			if (choice == 1)
				continue;
			choice--;
			continue;
		}
		if (c == 80)
		{
			if (choice == menu.size() - 1)
				continue;
			choice++;
			continue;
		}
		if (c == 27)
		{
			return menu.size();
		}
		if (c == 13)
		{
			return choice;
		}
	}
}

int input()
{
	int n;
	while (!(cin >> n) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Ошибка. Пожалуйста, используйте числовой ввод." << endl;
	}

	cin.ignore();
	return n;
}

void returnTime(int& day, int& month, int& year)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

	year = 1900 + ltm->tm_year;
	month = 1 + ltm->tm_mon;
	day = ltm->tm_mday;
}

float floatInput()
{
	float n;
	while (!(cin >> n) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Ошибка. Пожалуйста, используйте числовой ввод." << endl;
	}

	cin.ignore();
	return n;
}