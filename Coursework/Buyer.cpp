#include "Buyer.h"
#include "Order.h"
#include "Search.cpp"

bool compName(FlowerAdmin a, FlowerAdmin b)
{
	return a.name < b.name;
}

bool compPrice(FlowerAdmin a, FlowerAdmin b)
{
	if (a.price != b.price)
		return a.price < b.price;
	else
		return a.name < b.name;
}

bool compSale(FlowerAdmin a, FlowerAdmin b)
{
	if (a.sale != b.sale)
		return a.sale < b.sale;
	else if (a.price != b.price)
		return a.price < b.price;
	else
		return a.name < b.name;
}

void myOrders(User& user);

void menu_buyer(User& user, map <string, FlowerAdmin>& flowers, map <string, User>& users)
{
	vector <string> menu = { "МЕНЮ ПОКУПАТЕЛЯ",
													 "1 - Просто посмотреть товары",
													 "2 - Мои заказы",
													 "3 - Корзина",
													 "4 - Пополнить корзину",
													 "5 - Очистить корзину",
													 "6 - Выход" };

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
		{
			viewFlowersWithSort(flowers);
			system("pause");
			break;
		}

		case 2:
		{
			myOrders(user);
			system("pause");
			break;
		}

		case 3:
		{
			workCart(user.returnCart(), user.returnID());
			updateFileUsers(users);
			system("pause");
			break;
		}

		case 4:
		{
			topUpShoppingCart(flowers, user);
			updateFileUsers(users);
			system("pause");
			break;
		}

		case 5:
		{
			deleteCart(user);
			system("pause");
			break;
		}

		case 6:
			return;
		default:
		{
			vector <string> menuExit = { "Вы точно хотите выйти из учетной записи?",
											 "1 - Да, выйти",
											 "2 - Нет, продолжить работу" };
			choice = menu_helper(menuExit);
			switch (choice)
			{
			case 1:
				return;
			case 2:
				continue;
			}
		}
		}
	}
}

void myOrders(User& user)
{
	vector <Check> checks;

	ifstream in("checks", ios::binary);
	if (!in.is_open())
	{
		in.close();
		ofstream out("checks", ios::binary);
		out.close();
		ifstream in("checks", ios::binary);
	}
	else
		while (true)
		{
			Check check;
			in >> check;
			if (in.eof())
				break;
			checks.push_back(check);
		}
	in.close();

	bool flag = false;
	for (int i = 0; i < checks.size(); i++)
	{
		if (user.ID == checks[i].returnID())
		{
			cout << endl;
			if (!checks[i].returnPurchase())
			{
				cout << "Этот заказ еще в обработке: " << endl;
			}
			checkOut(checks[i], i + 1);
			flag = true;
			cout << endl;
		}
	}

	if (!flag)
	{
		cout << endl << "Вы еще ничего не заказывали в нашем магазине.Но вы всегда можете это сделать в соответствующем меню" << endl << endl;
		return;
	}

	return;
}

void topUpShoppingCart(map <string, FlowerAdmin>& flowers, User& user)
{
	while (true)
	{
		viewFlowers(flowers);

		int choice;
		while (true)
		{
			cout << "Введите номер добавляемого товара - ";
			choice = input();
			if (choice < 1 || choice > flowers.size())
			{
				cout << "Ошибка. Пожалуйста введите номер существующего товара" << endl << endl;
				continue;
			}
			break;
		}
		choice--;
		map <string, FlowerAdmin>::iterator it = flowers.begin();
		advance(it, choice);
		cout << endl << "Вы выбрали этот товар:" << endl << endl << it->second << endl << endl;

		int count;
		while (true)
		{
			cout << "Введите Количество добавляемого товара - ";
			count = input();
			if (count < 1)
			{
				cout << "Ошибка. Пожалуйста введите положительное число товаров" << endl << endl;
				continue;
			}
			bool flag = true;
			if (count > it->second.count)
			{
				cout << endl << "Вы хотите заказать товара больше, чем есть на складе." << endl << "Вам позвонит сотрудник компании и уточнит, может ли быть оформлен ваш заказ." << endl << "Возможно ваш заказ будет отменен. Уверены, что хотите продолжить? (1 - да, 2 - нет) -";
				while (flag)
				{
					int ch = input();
					if (ch < 1 || ch > 2)
					{
						cout << "Вы уверены, что хотите продолжить заказ? (1 - да, 2 - нет) - ";
						continue;
					}
					if (ch == 1)
						break;
					else
						flag = false;
				}
			}
			if (flag)
				break;
			else
				continue;
		}

		FlowerOrder flower(it->second.name, true, count, it->second.price, it->second.sale);
		user.cart.cart.push_back(flower);
		cout << endl << "Заказ успешно сформирован.\nВы добавили " << count << " товаров на сумму " << fixed << setprecision(2) << (it->second.price * count) - (it->second.price * count * it->second.sale) << "р." << endl << endl;
		user.cart.summaFull += (it->second.price * count) - (it->second.price * count * it->second.sale);
		user.cart.saleFull += it->second.price * count * it->second.sale;

		int ch;
		while (true)
		{
			cout << "Хотите добавить еще товаров? (1 - да, 2 - нет) - ";
			ch = input();
			if (ch < 1 || ch > 2)
			{
				continue;
			}
			break;
		}
		if (ch == 1)
			continue;
		else
			break;
	}
}

void deleteCart(User& user)
{
	user.cart.cart.clear();
	cout << endl << "Ваша корзина очищена" << endl << endl;
	return;
}

void viewVector(vector <FlowerAdmin> flowers)
{
	if (flowers.size() == 0)
	{
		cout << endl << "По вашему запросу ничего не найдено." << endl << endl;
		return;
	}

	int nameSize = 8;
	for (int i = 0; i < flowers.size(); i++)
	{
		nameSize = max(nameSize, flowers[i].name.size());
	}

	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << endl << setfill(' ');
	cout << "|    | " << setw(nameSize) << "Название" << " | На складе | Стоимость | Скидка | Дата привоза |" << endl;
	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << endl << setfill(' ');

	int k = 1;
	for (int i = 0; i < flowers.size(); i++)
	{
		cout << "| " << k++ << ". | " << setw(nameSize) << left << flowers[i].name << " | " << setw(9) << flowers[i].count << " | " << setw(9) << right << fixed << setprecision(2) << flowers[i].price << " | " << setw(5) << (int)(flowers[i].sale * 100) << "% |  " << setfill('0') << setw(2) << right << flowers[i].deliveryDay << '.' << setw(2) << flowers[i].deliveryMonth << '.' << setw(4) << flowers[i].deliveryYear << "  |" << setfill(' ') << left << endl;
		cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << setfill(' ') << endl;
	}
	return;
}

void viewFlowersWithSort(map <string, FlowerAdmin> fl)
{
	vector <FlowerAdmin> flowers;
	for (map <string, FlowerAdmin>::iterator it = fl.begin(); it != fl.end(); ++it)
	{
		flowers.push_back(it->second);
	}

	vector <string> menu = { "МЕНЮ ПРОСМОТРА",
													 "1 - Все",
													 "2 - Сортировка",
													 "3 - Поиск",
													 "4 - Фильтр" };

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
			viewFlowers(fl);
			system("pause");
			break;

		case 2:
		{
			vector <string> menuSort = { "МЕНЮ СОРТИРОВКИ",
											 "1 - По названию",
											 "2 - По цене",
											 "3 - По скидке" };
			bool fl = true;
			while (fl)
			{
				fl = true;
				int ch = menu_helper(menuSort);

				switch (ch)
				{
				case 1:
					sort(flowers.begin(), flowers.end(), compName);
					viewVector(flowers);
					system("pause");
					break;

				case 2:
					sort(flowers.begin(), flowers.end(), compPrice);
					viewVector(flowers);
					system("pause");
					break;
				case 3:
					sort(flowers.begin(), flowers.end(), compSale);
					viewVector(flowers);
					system("pause");
					break;
				default:
					fl = false;
				}
			}
			break;
		}

		case 3:
		{
			vector <string> menuSearch = { "МЕНЮ ПОИСКА",
											 "1 - По названию",
											 "2 - По дате" };

			vector <FlowerAdmin> flowerSearch;
			string s;

			bool flag = true;
			while (flag)
			{
				int chh = menu_helper(menuSearch);

				switch (chh)
				{
				case 1:
				{
					while (true)
					{
						cout << "Введите искомое название - ";
						getline(cin, s);
						if (s.size() == 0)
						{
							cout << "Название не может быть пустым." << endl << endl;
							continue;
						}
						break;
					}

					s = Search::search <string>::toLower(s);
					for (int i = 0; i < flowers.size(); i++)
					{
						string name = Search::search <string> ::toLower(flowers[i].name);
						if (name.find(s) != string::npos)
							flowerSearch.push_back(flowers[i]);
					}

					viewVector(flowerSearch);
					flowerSearch.clear();
					system("pause");
					break;
				}

				case 2:
				{
					string time;
					int day, month, year;

					while (true)
					{
						time = "";
						cout << "Введите дату в формате ДД.ММ.ГГГГ - ";

						char c;
						while (true)
						{
							c = _getch();
							if (c == 8)
							{
								if (time.size() == 0)
									continue;
								if (time.size() == 2 || time.size() == 4)
								{
									cout << "\b \b";
									cout << "\b \b";
									time.erase(time.size() - 1, 1);
									continue;
								}
								cout << "\b \b";
								time.erase(time.size() - 1, 1);
								continue;
							}
							if (c == '\r')
								if (time.size() == 8)
									break;
							if (c < '0' || c > '9')
								continue;
							if (time.size() == 8)
								continue;
							time += c;
							cout << c;
							if (time.size() == 2 || time.size() == 4)
								cout << '.';
						}

						day = (time[0] - '0') * 10 + (time[1] - '0');
						month = (time[2] - '0') * 10 + (time[3] - '0');
						year = (time[4] - '0') * 1000 + (time[5] - '0') * 100 + (time[6] - '0') * 10 + (time[7] - '0');
						if (day == 0 || day > 31 || month == 0 || month > 12 || year < 2022)
						{
							cout << endl << "Ошибка. Невозможная дата. Попробуйте еще раз." << endl << endl;
							continue;
						}

						if ((month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && day == 31)
						{
							cout << endl << "Ошибка. Невозможная дата. Попробуйте еще раз." << endl << endl;
							continue;
						}

						if (month == 2 && day > 29)
						{
							cout << endl << "Ошибка. Невозможная дата. Попробуйте еще раз." << endl << endl;
							continue;
						}

						if (!(year % 4 == 0 && year % 400 == 0 && year % 100 != 0) && month == 2 && day == 29)
						{
							cout << endl << "Ошибка. Невозможная дата (29 февраля в невисокосном году). Попробуйте еще раз." << endl << endl;
							continue;
						}

						cout << endl << endl;
						break;
					}

					for (int i = 0; i < flowers.size(); i++)
					{
						if (flowers[i].deliveryDay == day && flowers[i].deliveryMonth == month && flowers[i].deliveryYear == year)
							flowerSearch.push_back(flowers[i]);
					}

					viewVector(flowerSearch);
					flowerSearch.clear();
					system("pause");
					break;
				}

				default:
					flag = false;
				}
			}

			break;
		}

		case 4:
		{
			vector <string> menuFiltr = { "МЕНЮ ФИЛЬТРАЦИИ",
											 "1 - Цена от - до",
											 "2 - Только со скидкой",
											 "3 - Без скидки",
											 "4 - По тексту" };
			vector <FlowerAdmin> flowerSearch;
			bool fl = true;
			while (fl)
			{
				int ch = menu_helper(menuFiltr);

				switch (ch)
				{
				case 1:
				{
					int c1, c2;
					while (true)
					{
						cout << "Введите минимальную цену - ";
						c1 = input();
						if (c1 < 0)
						{
							cout << "Цена не может быть отрицательной. Пожалуйста, повторите попытку" << endl << endl;
							continue;
						}
						break;
					}
					while (true)
					{
						cout << "Введите максимальную цену - ";
						c2 = input();
						if (c2 < 0)
						{
							cout << "Цена не может быть отрицательной. Пожалуйста, повторите попытку" << endl << endl;
							continue;
						}
						break;
					}

					for (int i = 0; i < flowers.size(); i++)
					{
						if (flowers[i].price >= c1 && flowers[i].price <= c2)
							flowerSearch.push_back(flowers[i]);
					}
					viewVector(flowerSearch);
					flowerSearch.clear();
					system("pause");
					break;
				}

				case 2:
				{
					for (int i = 0; i < flowers.size(); i++)
					{
						if (flowers[i].sale > 0)
							flowerSearch.push_back(flowers[i]);
					}
					viewVector(flowerSearch);
					flowerSearch.clear();
					system("pause");
					break;
				}

				case 3:
				{
					for (int i = 0; i < flowers.size(); i++)
					{
						if (flowers[i].sale == 0)
							flowerSearch.push_back(flowers[i]);
					}
					viewVector(flowerSearch);
					flowerSearch.clear();
					system("pause");
					break;
				}

				case 4:
				{
					vector <FlowerAdmin> flowerSearch;
					string s;
					while (true)
					{
						cout << "Введите искомую часть текста - ";
						getline(cin, s);
						if (s.size() == 0)
						{
							cout << "Название не может быть пустым." << endl << endl;
							continue;
						}
						break;
					}

					s = Search::search <string>::toLower(s);
					for (int i = 0; i < flowers.size(); i++)
					{
						string name = Search::search <string> ::toLower(flowers[i].name);
						if (name.find(s) != string::npos)
							flowerSearch.push_back(flowers[i]);
					}

					viewVector(flowerSearch);
					system("pause");
					break;
				}

				default:
					fl = false;
				}
			}



			break;
		}

		default:
			return;
		}
	}
}