#include "Admin.h"

void Stat();

class Flow
{
	string name;
public:
	class Money
	{
		float expenses, parishes;
	public:
		Money()
		{
			this->expenses = 0;
			this->parishes = 0;
		}
		friend void Stat();
		void setExp(float expenses)
		{
			this->expenses += expenses;
		}
		void setPar(float parishes)
		{
			this->parishes += parishes;
		}
	};
	Money money;

	Flow(string name)
	{
		this->name = name;
	}
	void setExp(float expenses)
	{
		this->money.setExp(expenses);
	}

	void setPar(float parishes)
	{
		this->money.setPar(parishes);
	}
	friend void Stat();
};

void menu_admin(map <string, Account>& accounts, map <string, User>& users, map <string, FlowerAdmin>& flowers, string login)
{
	vector <string> menu = { "МЕНЮ АДМИНИСТРАТОРА",
													 "1 - Управление учетными записями",
													 "2 - Управление учетными записями пользователей",
													 "3 - Управление товаром",
													 "4 - Статистика дохода",
													 "5 - Выход" };

	while (true)
	{
		int choice = menu_helper(menu);
		switch (choice)
		{
		case 1:
			menu_accountsManagement(accounts, login);
			break;
		case 2:
			menu_usersManagement(users);
			break;
		case 3:
			menu_flowerManagement(flowers);
			break;
		case 4:
			Stat();
			system("pause");
			break;
		case 5:
			return;
			break;
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

void menu_accountsManagement(map<string, Account>& accounts, string login)
{
	vector <string> menu = { "УПРАВЛЕНИЕ УЧЕТНЫМИ ЗАПИСЯМИ",
													 "1 - Просмотр",
													 "2 - Добавление",
													 "3 - Редактирование",
													 "4 - Удаление" };

	while (true)
	{
		int choice = menu_helper(menu);
		switch (choice)
		{
		case 1:
		{
			vector <string> choiceView = { "МЕНЮ ПРОСМОТРА УЧЕТНЫХ ЗАПИСЕЙ",
													 "1 - Все",
													 "2 - Администраторы",
													 "3 - Сотрудники магазина" };

			while (true)
			{
				choice = menu_helper(choiceView);
				if (choice == 4)
					break;
				viewAccounts(accounts, choice, login);
				system("pause");
			}
			break;
		}
		case 2:
		{
			string name, login, password;
			bool role = false;
			while (true)
			{
				system("cls");
				cout << "МЕНЮ ДОБАВЛЕНИЯ НОВОГО РАБОТНИКА" << endl << endl;

				while (true)
				{
					name = "";
					cout << "Введите ФИО (не более 50 символов) - ";
					getline(cin, name);
					if (name.size() == 0)
					{
						cout << "Имя не может быть пустым. Повторите попытку" << endl;
						continue;
					}
					if (name.size() > 50)
					{
						cout << "Имя работника слишком длинное. Пожалуйста, используйте не более 50 символов." << endl;
						continue;
					}
					else
						break;
				}

				while (true)
				{
					login = "";
					cout << "Введите логин (не более 20 символов) - ";
					getline(cin, login);
					if (login.size() == 0)
					{
						cout << "Логин не может быть пустым. Повторите попытку" << endl;
						continue;
					}
					map <string, Account> ::iterator it;
					it = accounts.find(login);
					if (it != accounts.end())
					{
						cout << "Пользователь с таким логином уже существует. Попробуйте еще раз." << endl;
						continue;
					} if (login.size() > 20)
					{
						cout << "Логин слишком длинный. Пожалуйста, используйте не более 20 символов." << endl;
					}
					else
						break;
				}

				while (true)
				{
					password = "";
					cout << "Введите пароль (минимум 5 символов) - ";
					getline(cin, password);
					if (password.size() == 0)
					{
						cout << "Пароль не может быть пустым. Повторите попытку" << endl;
						continue;
					}
					if (password.size() < 5)
					{
						cout << "Ваш пароль слишком короткий. Попробуйте еще раз, используя минимум 5 символов." << endl;
						continue;
					}
					else
						break;
				}

				while (true)
				{
					string passwordCheck = "";
					cout << "Повторите пароль - ";
					getline(cin, passwordCheck);
					if (passwordCheck != password)
					{
						cout << "Пароли не совпадают. Попробуйте еще раз." << endl;
						continue;
					}
					else
						break;
				}

				while (true)
				{
					int i;
					cout << "Введите роль (Админ - 1, Продавец - 2) - ";

					i = input();
					if (i < 1 || i > 2)
					{
						cout << "Введите цифры 1 или 2. Попробуйте еще раз." << endl;
						continue;
					}
					else
					{
						if (i == 1)
							role = true;
						else
							role = false;
						break;
					}
				}

				Account acc(name, login, password, role);
				ofstream out("accounts", ios::binary | ios::app);
				out << acc;
				out.close();
				accounts.insert(make_pair(login, acc));

				cout << endl << acc << endl;
				cout << "Пользователь успешно добавлен.\nТеперь он может войти в свою учетную запись." << endl << endl;
				system("pause");
				break;
			}
			break;
		}
		case 3:
		{
			viewAccounts(accounts, 1, "");

			while (true)
			{
				cout << endl << "Введите номер аккаунта для редактирования - ";
				int i = input();
				if (i < 1 || i > accounts.size())
				{
					cout << "Попробуйте еще раз." << endl;
					continue;
				}
				i--;
				map <string, Account> ::iterator it = accounts.begin();
				advance(it, i);
				string newLogin = it->second.Edit(accounts);
				if (it->first != newLogin)
				{
					Account acc;
					acc = it->second;
					accounts.erase(it);
					accounts.insert(make_pair(newLogin, acc));
					break;
				}
				else
					break;
			}

			updateFileAccounts(accounts);
			break;
		}
		case 4:
		{
			viewAccounts(accounts, 1, login);

			while (true)
			{
				cout << endl << "Введите номер аккаунта для удаления - ";
				int i = input();
				if (i < 1 || i > accounts.size())
				{
					cout << "Попробуйте еще раз." << endl;
					continue;
				}
				i--;
				map <string, Account> ::iterator it = accounts.begin();
				advance(it, i);

				if (it->second.returnLogin() == login)
				{
					cout << endl << "ВНИМАНИЕ. Вы не можете удалить свой аккаунт." << endl << endl;
					system("pause");
					break;
				}

				accounts.erase(it);
				break;
			}
			updateFileAccounts(accounts);
			cout << endl << "Аккаунт успешно удален." << endl << endl;
			system("pause");
			break;
		}
		default:
			return;
		}
	}
}

void menu_usersManagement(map<string, User>& users)
{
	vector <string> menu = { "УПРАВЛЕНИЕ УЧЕТНЫМИ ЗАПИСЯМИ",
													 "1 - Просмотр",
													 "2 - Удаление" };

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
		{
			viewUsers(users);
			system("pause");
			break;
		}
		case 2:
		{
			viewUsers(users);

			while (true)
			{
				cout << endl << "Введите номер аккаунта для удаления - ";
				int i = input();
				if (i < 1 || i > users.size())
				{
					cout << "Попробуйте еще раз." << endl;
					continue;
				}
				i--;
				map <string, User> ::iterator it = users.begin();
				advance(it, i);

				users.erase(it);
				break;
			}
			updateFileUsers(users);
			cout << endl << "Аккаунт успешно удален." << endl << endl;
			system("pause");
			break;
		}
		default:
			return;
		}
	}
}

void menu_flowerManagement(map<string, FlowerAdmin>& flowers)
{
	vector <string> menu = { "УПРАВЛЕНИЕ ТОВАРНЫМИ ЕДИНИЦАМИ",
													 "1 - Просмотр",
													 "2 - Добавление",
													 "3 - Редактирование",
													 "4 - Удаление" };

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
		{
			viewFlowers(flowers);
			system("pause");
			break;
		}
		case 2:
		{
			addFlower(flowers);
			break;
		}

		case 3:
		{
			viewFlowers(flowers);
			cout << endl;

			int i;
			while (true)
			{
				cout << "Введите номер товара для редактирования - ";
				i = input();

				if (i < 1 || i > flowers.size())
				{
					cout << "ОШИБКА. Введите действительное число нумерации товара." << endl;
					continue;
				}
				else
				{
					i--;
					break;
				}
			}

			map <string, FlowerAdmin> ::iterator it = flowers.begin();
			advance(it, i);
			string newName;
			newName = Editflower(flowers, it);
			if (it->first != newName)
			{
				FlowerAdmin flower;
				flower = it->second;
				flowers.erase(it);
				flowers.insert(make_pair(flower.returnName(), flower));
			}

			updateFileFlowers(flowers);
			break;
		}

		case 4:
		{
			viewFlowers(flowers);
			cout << endl;

			int i;
			while (true)
			{
				cout << "Введите номер товара для удаления - ";
				i = input();

				if (i < 1 || i > flowers.size())
				{
					cout << "ОШИБКА. Введите действительное число нумерации товара." << endl;
					continue;
				}
				else
				{
					i--;
					break;
				}
			}

			map <string, FlowerAdmin> ::iterator it = flowers.begin();
			advance(it, i);
			flowers.erase(it);
			updateFileFlowers(flowers);
			cout << endl << "Товар успешно удален" << endl << endl;
			system("pause");
			break;
		}

		default:
			return;
		}
	}
}

void viewAccounts(map<string, Account>& accounts, int choice, string login)
{
	if (choice == 1)
		cout << endl << "ВСЕ" << endl;
	if (choice == 2)
		cout << endl << "АДМИНИСТРАТОРЫ" << endl;
	if (choice == 3)
		cout << endl << "СОТРУДНИКИ МАГАЗИНА" << endl;

	int loginSize = 1;
	int nameSize = 1;
	for (map <string, Account> ::iterator it = accounts.begin(); it != accounts.end(); ++it)
	{
		if (choice == 2 && !it->second.role)
			continue;
		if (choice == 3 && it->second.role)
			continue;
		loginSize = max(loginSize, it->second.login.size());
		nameSize = max(nameSize, it->second.name.size());
	}

	cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;
	cout << "|#п/п|  Тип аккаунта  | " << setfill(' ') << setw(nameSize) << left << "Имя" << " | " << setfill(' ') << setw(loginSize) << "Логин" << " |" << endl;
	cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;

	int i = 1;
	for (map <string, Account> ::iterator it = accounts.begin(); it != accounts.end(); it++)
	{
		if (choice == 2 && !it->second.role)
			continue;
		if (choice == 3 && it->second.role)
			continue;
		cout << "| " << i++ << ". | " << setfill(' ') << (it->second.role ? "Администратор " : "Сотрудник маг.") << " | " << setw(nameSize) << left << it->second.name << " | " << setw(loginSize) << it->second.login << " |";
		if (it->second.login == login)
			cout << " << Ваш аккаунт";
		cout << endl;
		cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;
		cout << setfill(' ');
	}
	return;
}

void viewUsers(map<string, User>& users)
{
	if (users.size() == 0)
	{
		cout << endl << "Акканты пользователей отсутствуют в системе." << endl << endl;
		return;
	}

	int loginSize = 6;
	int nameSize = 5;
	for (map <string, User> ::iterator it = users.begin(); it != users.end(); ++it)
	{
		loginSize = max(loginSize, it->second.login.size());
		nameSize = max(nameSize, it->second.name.size());
	}

	cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;
	cout << "|#п/п|  Тип аккаунта  | " << setfill(' ') << setw(nameSize) << left << "Имя" << " | " << setfill(' ') << setw(loginSize) << "Логин" << " |" << endl;
	cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;

	int i = 1;
	for (map <string, User> ::iterator it = users.begin(); it != users.end(); it++)
	{
		cout << "| " << i++ << ". | " << setfill(' ') << " Пользователь  | " << setw(nameSize) << left << it->second.name << " | " << setw(loginSize) << it->second.login << " |" << endl;
		cout << "|----|----------------|-" << setfill('-') << setw(nameSize) << "" << "-|-" << setfill('-') << setw(loginSize) << "" << "-|" << endl;
		cout << setfill(' ');
	}
	cout << endl;
	return;
}

void viewFlowers(map<string, FlowerAdmin>& flowers)
{
	if (flowers.size() == 0)
	{
		cout << endl << "Данные о товарах отсутствуют в системе." << endl << endl;
		return;
	}

	int nameSize = 8;
	for (map <string, FlowerAdmin>::iterator it = flowers.begin(); it != flowers.end(); it++)
	{
		nameSize = max(nameSize, it->second.name.size());
	}

	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << endl << setfill(' ');
	cout << "|    | " << setw(nameSize) << "Название" << " | На складе | Стоимость | Скидка | Дата привоза |" << endl;
	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << endl << setfill(' ');

	int i = 1;
	for (map <string, FlowerAdmin>::iterator it = flowers.begin(); it != flowers.end(); it++)
	{
		cout << "| " << i++ << ". | " << setw(nameSize) << left << it->second.name << " | " << setw(9) << it->second.count << " | " << setw(9) << right << fixed << setprecision(2) << it->second.price << " | " << setw(5) << (int)(it->second.sale * 100) << "% |  " << setfill('0') << setw(2) << right << it->second.deliveryDay << '.' << setw(2) << it->second.deliveryMonth << '.' << setw(4) << it->second.deliveryYear << "  |" << setfill(' ') << left << endl;
		cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|-----------|--------|--------------|" << setfill(' ') << endl;
	}
	return;
}

string Editflower(map<string, FlowerAdmin>& flowers, map<string, FlowerAdmin>::iterator flowerIt)
{
	vector <string> menu = { "Что вы хотите отредактировать?",
													 "1 - Название",
													 "2 - Цену",
													 "3 - Скидку" };

	while (true)
	{
		int choice = menu_helper(menu);
		cout << "Вы редактируете этот товар: " << endl << endl << flowerIt->second << endl;

		switch (choice)
		{
		case 1:
		{
			while (true)
			{
				string name;
				while (true)
				{
					cout << "Введите новое название - ";
					getline(cin, name);
					if (name.size() == 0)
					{
						cout << "Название не может быть пустым. Повторите попытку" << endl;
						continue;
					}
					if (name.size() == 0)
					{
						cout << "Название не может быть пустым. Повторите попытку" << endl;
						continue;
					}
					break;
				}

				map <string, FlowerAdmin>::iterator it = flowers.find(name);
				if (it != flowers.end())
				{
					cout << "Такое название уже есть. Попробуйте еще раз" << endl;
					continue;
				}
				flowerIt->second.name = name;
				break;
			}
			cout << endl << "Теперь этот товар выглядит так: " << endl << endl << flowerIt->second << endl;
			cout << "Выйдите из меню редактирования для сохранения изменений" << endl << endl;
			system("pause");
			break;
		}

		case 2:
		{
			while (true)
			{
				float price;
				cout << "Введите новую цену - ";
				price = floatInput();
				if (price <= 0)
				{
					cout << "ОШИБКА. Введите целое положительное число" << endl << endl;
					continue;
				}
				if (price >= 1000000)
				{
					cout << "ОШИБКА. Слишком большая цена (максимум - 999999.99)" << endl << endl;
					continue;
				}
				flowerIt->second.price = price;
				break;
			}
			cout << endl << "Теперь этот товар выглядит так: " << endl << endl << flowerIt->second << endl;
			cout << "Выйдите из меню редактирования для сохранения изменений" << endl << endl;
			system("pause");
			break;
		}

		case 3:
		{
			while (true)
			{
				cout << "Какую скидку вы хотите выставить на товар? - ";
				int s = input();
				if (s < 0 || s > 100)
				{
					cout << "ОШИБКА. Скидка может быть от 0 до 100%" << endl << endl;
					continue;
				}

				flowerIt->second.sale = s / 100.f;
				break;
			}
			cout << endl << "Теперь этот товар выглядит так: " << endl << endl << flowerIt->second << endl;
			cout << "Выйдите из меню редактирования для сохранения изменений" << endl << endl;
			system("pause");
			break;
		}

		default:
			return flowerIt->second.name;
		}
	}
}

void addFlower(map<string, FlowerAdmin>& flowers)
{
	vector <string> menu = { "УПРАВЛЕНИЕ ЗАКАЗА",
													 "1 - Заказ уже существующего товара",
													 "2 - Заказ нового товара" };
	bool flag = true;

	while (true)
	{
		int choice = menu_helper(menu);

		switch (choice)
		{
		case 1:
		{
			viewFlowers(flowers);

			cout << endl;

			int i;
			while (true)
			{
				cout << "Введите номер товара для добавления - ";
				i = input();

				if (i < 1 || i > flowers.size())
				{
					cout << "ОШИБКА. Введите действительное число нумерации товара." << endl;
					continue;
				}
				else
				{
					i--;
					break;
				}
			}

			map <string, FlowerAdmin> ::iterator it = flowers.begin();
			advance(it, i);
			system("cls");
			cout << "Вы пополняете этот товар:" << endl << endl << it->second << endl;

			int count;
			while (true)
			{
				cout << "Сколько товара вы хотите заказать? - ";
				count = input();
				if (count <= 0)
				{
					cout << "ОШИБКА. Введите целое неотрицательно число сколько вы хотите заказать." << endl << endl;
					continue;
				}

				break;
			}

			while (true)
			{
				cout << "По какой цене вы заказали? - ";
				float price = floatInput();
				if (price <= 0)
				{
					cout << "ОШИБКА. Введите целое положительное число" << endl << endl;
					continue;
				}
				if (price >= 1000000)
				{
					cout << "ОШИБКА. Слишком большая цена (максимум - 999999.99)" << endl << endl;
					continue;
				}

				int day, month, year;
				returnTime(day, month, year);
				it->second.request(count, day, month, year);
				updateFileFlowers(flowers);
				cout << endl << it->second << endl << "Количество товара успешно пополнено." << endl << endl;
				it->second.updateExpensesFile(count, price);
				system("pause");
				break;
			}

			break;
		}

		case 2:
		{
			string name;
			while (true)
			{
				cout << endl << "Введите название товара - ";
				getline(cin, name);
				if (name.size() == 0)
				{
					cout << "Название не может быть пустым. Повторите попытку" << endl;
					continue;
				}
				break;
			}
			if (name[0] >= 'a' && name[0] <= 'z')
				name[0] = name[0] + ('Z' - 'z');
			if (name[0] >= 'а' && name[0] <= 'я')
				name[0] = name[0] + ('Я' - 'я');

			map <string, FlowerAdmin>::iterator it = flowers.find(name);
			if (it != flowers.end())
			{
				cout << endl << "Такой товар присутствует в системе. Пожалуйста, используйте меню заказа уже существующих товаров" << endl << endl;
				system("pause");
				break;
			}

			int count;
			while (true)
			{
				cout << "Сколько товара вы хотите заказать? - ";
				count = input();
				if (count <= 0)
				{
					cout << "ОШИБКА. Введите целое неотрицательно число сколько вы хотите заказать." << endl << endl;
					continue;
				}

				break;
			}

			float price;
			while (true)
			{
				cout << "По какой цене вы заказали? - ";
				price = floatInput();
				if (price <= 0)
				{
					cout << "ОШИБКА. Введите целое положительное число" << endl << endl;
					continue;
				}
				if (price >= 1000000)
				{
					cout << "ОШИБКА. Слишком большая цена (максимум - 999999.99)" << endl << endl;
					continue;
				}
				break;
			}

			float sale;
			while (true)
			{
				cout << "Какую скидку вы хотите выставить на товар? - ";
				int s = input();
				if (s < 0 || s > 100)
				{
					cout << "ОШИБКА. Скидка может быть от 0 до 100%" << endl << endl;
					continue;
				}

				sale = s / 100.f;
				break;
			}

			int day, month, year;
			returnTime(day, month, year);
			FlowerAdmin flower(name, count, price + price * 0.4, sale, day, month, year);
			flowers.insert(make_pair(name, flower));
			ofstream out("flowers", ios::binary | ios::app);
			out << flower;
			out.close();
			flower.updateExpensesFile(count, price);
			cout << "Товар успешно добавлен" << endl << endl << flower << endl;
			system("pause");
			break;
		}

		default:
			return;
		}
	}

}

void Stat()
{
	vector <Check> checks;
	map <string, Flow> flowers;
	map <string, float> topPrice;
	map <string, int> topCount;
	pair <string, int> cnt;
	pair <string, float> pr;

	cnt.second = 0;
	pr.second = 0;

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
		if (check.returnPurchase())
			checks.push_back(check);
	}
	in.close();

	for (int i = 0; i < checks.size(); i++)
	{
		for (int j = 0; j < checks[i].cart.size(); j++)
		{
			if (flowers.find(checks[i].cart[j].name) == flowers.end())
			{
				Flow flow(checks[i].cart[j].name);
				flowers.insert(make_pair(checks[i].cart[j].name, flow));
			}
			flowers.find(checks[i].cart[j].name)->second.setPar(checks[i].cart[j].price * checks[i].cart[j].count - checks[i].cart[j].price * checks[i].cart[j].count * checks[i].cart[j].sale);

			if (topCount.find(checks[i].cart[j].name) == topCount.end())
			{
				topCount.insert(make_pair(checks[i].cart[j].name, 0));
			}
			topCount.find(checks[i].cart[j].name)->second += checks[i].cart[j].count;
		}
	}

	float price, Fullexpenses = 0, fullParishes = 0;
	int count;
	string name;
	in.open("expenses", ios::binary);
	while (true)
	{
		in.read((char*)&count, sizeof(count));
		if (in.eof())
			break;
		in.read((char*)&price, sizeof(price));

		size_t len;
		char* buf;

		in.read((char*)&len, sizeof(len));
		buf = new char[len];
		in.read(buf, len);
		name = buf;                              //считывание имени
		delete[] buf;

		if (flowers.find(name) == flowers.end())
		{
			Flow flow(name);
			flowers.insert(make_pair(name, flow));
		}

		flowers.find(name)->second.setExp(count * price);
	}

	in.close();

	int nameSize = 8;
	for (map <string, Flow>::iterator it = flowers.begin(); it != flowers.end(); ++it)
	{
		nameSize = max(nameSize, it->second.name.size());
	}

	cout << "|-" << setfill('-') << setw(nameSize) << "" << "-|------------|-------------|" << setfill(' ') << endl;
	cout << "| " << setw(nameSize) << "Название" << " |   Расход   |   Приход    |" << endl;
	cout << "|-" << setfill('-') << setw(nameSize) << "" << "-|------------|-------------|" << setfill(' ') << endl;


	for (map <string, Flow>::iterator it = flowers.begin(); it != flowers.end(); ++it)
	{
		Fullexpenses += it->second.money.expenses;
		fullParishes += it->second.money.parishes;

		if (topPrice.find(it->first) == topPrice.end())
		{
			topPrice.insert(make_pair(it->first, 0));
		}
		topPrice.find(it->first)->second += it->second.money.parishes;

		cout << "| " << setw(nameSize) << it->second.name << " | " << fixed << setprecision(2) << setw(10) << it->second.money.expenses << " | " << fixed << setprecision(2) << setw(11) << it->second.money.parishes << " |" << endl;
		cout << "|-" << setfill('-') << setw(nameSize) << "" << " |------------|-------------|" << setfill(' ') << endl;
	}
	cout << endl << "Вы заработали " << fullParishes << "р." << endl;
	cout << "Вы потратили " << Fullexpenses << "р." << endl << endl;

	for (map <string, int>::iterator it = topCount.begin(); it != topCount.end(); ++it)
	{
		if (it->second > cnt.second)
		{
			cnt.first = it->first;
			cnt.second = it->second;
		}
	}

	for (map <string, float>::iterator it = topPrice.begin(); it != topPrice.end(); ++it)
	{
		if (it->second > pr.second)
		{
			pr.first = it->first;
			pr.second = it->second;
		}
	}

	if (cnt.first.size() != 0)
		cout << "Самый продаваемый товар - " << cnt.first << endl;
	if (pr.first.size() != 0)
		cout << "Самый прибыльный товар - " << pr.first << endl;
	if (fullParishes < Fullexpenses)
		cout << endl << "Вы уходите в минус. Такими темпами вы обанкротитесь" << endl;
	else
		cout << endl << "Вы выходите в плюс. Поздравляю" << endl;


	ofstream out("statistic.txt", ios::trunc);

	out << "|-" << setfill('-') << setw(nameSize) << "" << "-|------------|-------------|" << setfill(' ') << endl;
	out << "| " << setw(nameSize) << "Название" << " |   Расход   |   Приход    |" << endl;
	out << "|-" << setfill('-') << setw(nameSize) << "" << "-|------------|-------------|" << setfill(' ') << endl;

	for (map <string, Flow>::iterator it = flowers.begin(); it != flowers.end(); ++it)
	{
		out << "| " << setw(nameSize) << it->second.name << " | " << fixed << setprecision(2) << setw(10) << it->second.money.expenses << " | " << fixed << setprecision(2) << setw(11) << it->second.money.parishes << " |" << endl;
		out << "|-" << setfill('-') << setw(nameSize) << "" << "-|------------|-------------|" << setfill(' ') << endl;
	}
	out << endl << "Вы заработали " << fullParishes << "р." << endl;
	out << "Вы потратили " << Fullexpenses << "р." << endl << endl;

	if (cnt.first.size() != 0)
		out << "Самый продаваемый товар - " << cnt.first << endl;
	if (pr.first.size() != 0)
		out << "Самый прибыльный товар - " << pr.first << endl;

	if (fullParishes < Fullexpenses)
		out << endl << "Вы уходите в минус. Такими темпами вы обанкротитесь" << endl;
	else
		out << endl << "Вы выходите в плюс. Поздравляю" << endl;

	out.close();
	return;
}
