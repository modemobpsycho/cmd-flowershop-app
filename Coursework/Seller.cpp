#include "Seller.h"

void menu_seller(map <string, FlowerAdmin>& flowers)
{
	vector <string> menu = { "МЕНЮ ПРОДАВЦА",
													 "1 - Просмотр заказов",
													 "2 - Сборка заказа",
													 "3 - Выход" };

	vector <Check> checks;
	vector <Check> checksFull;
	ifstream in("checks", ios::binary);
	if (!in.is_open())
	{
		in.close();
		ofstream out("checks", ios::binary | ios::app);
		out.close();
		ifstream in("checks", ios::binary);
	}

	while (true)
	{
		Check check;
		in >> check;
		if (in.eof())
			break;
		if (!check.returnPurchase())
			checks.push_back(check);
		else
			checksFull.push_back(check);
	}
	in.close();

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
		{
			if (checks.size() == 0)
			{
				cout << "В системе не обнаружены заказы" << endl << endl;
				system("pause");
				break;
			}
			cout << endl;
			for (int i = 0; i < checks.size(); i++)
			{
				cout << endl;
				checkOut(checks[i], i + 1);
				cout << endl;
			}
			system("pause");
			break;
		}

		case 2:
		{
			if (checks.size() == 0)
			{
				cout << "В системе не обнаружены заказы" << endl << endl;
				system("pause");
				break;
			}
			cout << endl;
			for (int i = 0; i < checks.size(); i++)
			{
				cout << endl;
				checkOut(checks[i], i + 1);
				cout << endl;
			}

			int ch;
			while (true)
			{
				cout << "Введите номер подтверждаемого заказа - ";
				ch = input();
				if (ch < 1 || ch > checks.size())
				{
					cout << "Введите существующий номер заказа" << endl << endl;
					continue;
				}
				break;
			}
			ch--;

			shared_ptr <Check> ptr(new Check);
			*ptr = checks[ch];
			checks[ch] = placeOrder(ptr, flowers);
			if (checks[ch].returnPurchase())
			{
				checksFull.push_back(checks[ch]);
				checks.erase(checks.begin() + ch);
			}
			updateFileChecks(checks, checksFull);
			system("pause");
			break;
		}
		default:
			return;
		}
	}
}

void updateFileChecks(vector<Check>& checks, vector<Check>& checksFull)
{
	ofstream out("checks", ios::binary | ios::trunc);
	for (int i = 0; i < checks.size(); i++)
		out << checks[i];
	for (int i = 0; i < checksFull.size(); i++)
		out << checksFull[i];
	out.close();
	return;
}

Check placeOrder(shared_ptr <Check> ptr, map <string, FlowerAdmin>& flowers)
{
	bool flag = true;
	for (int i = 0; i < ptr->cart.size(); i++)
	{
		if (flowers.find(ptr->cart[i].name) == flowers.end())
		{
			flag = false;
		}
		if (flowers.find(ptr->cart[i].name)->second.count < ptr->cart[i].count)
		{
			flag = false;
		}
	}

	if (!flag)
	{
		cout << "Вы не можете оформить этот заказа\nОдного или нескольких товаров не хватает на складе\nПопросите администратора дозаказать товары" << endl << endl;
		return *ptr;
	}

	for (int i = 0; i < ptr->cart.size(); i++)
	{
		flowers.find(ptr->cart[i].name)->second.count -= ptr->cart[i].count;
	}
	ptr->purchase = true;

	updateFileFlowers(flowers);
	return *ptr;
}
