#include "Authentification.h"


void authentification(map <string, Account>& accounts, map <string, User>& users, map <string, FlowerAdmin>& flowers)
{
	string login, password;


	while (true)
	{
		cout << "Введите логин - ";
		getline(cin, login);
		try
		{
			if (login.size() == 0)
				throw exception("Логин не может быть пустой");
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			continue;
		}
	}

	cout << "Введите пароль - ";
	char c;
	while (true)
	{
		c = _getch();
		if (c == 8)
		{
			if (password.size() == 0)
				continue;
			cout << "\b \b";
			password.erase(password.size() - 1, 1);
			continue;
		}
		if (c == '\r')
			break;
		password += c;
		cout << '*';
	}
	cout << endl;

	map <string, Account> ::iterator it;
	it = accounts.find(login);

	if (it == accounts.end())
	{
		map <string, User> ::iterator userIt;
		userIt = users.find(login);
		if (userIt == users.end())
		{
			cout << "Аккаунта с таким логином нет. Желаете зарегистрироваться?" << endl;
			int answer;
			while (true)
			{
				cout << "1 - да, 2 - нет : ";
				answer = input();
				if (answer < 1 || answer > 2)
				{
					cout << "Ошибка, попробуйте еще раз." << endl << "Желаете зарегистрироваться?" << endl;
					continue;
				}
				else
					break;
			}
			if (answer == 1)
			{
				user_registration(users);
				return;
			}
			else
				return;
		}
		else
		{
			menu_buyer(userIt->second, flowers, users);
		}
	}
	else
	{
		if (it->second.returnPassword(password))
		{
			if (it->second.returnRole())
			{
				menu_admin(accounts, users, flowers, login);
			}
			else
			{
				menu_seller(flowers);
				return;
			}
		}
		else
		{
			cout << "Неверный логин и/или пароль. Попробуйте еще раз." << endl << endl;
			system("pause");
			return;
		}
	}
}

void user_registration(map <string, User>& users)
{
	string login, password, passwordCheck, name;

	while (true)
	{
		system("cls");
		login = "";
		password = "";
		name = "";
		passwordCheck = "";

		cout << "------Добро пожаловать в меню регистрации------" << endl << endl;

		while (true)
		{
			cout << "Как вас зовут? - ";
			getline(cin, name);
			if (name.size() == 0)
			{
				cout << "Имя не может быть пустым. Повторите попытку" << endl;
				continue;
			}
			break;
		}

		while (true)
		{
			login = "";
			cout << "Придумайте логин - ";
			getline(cin, login);
			if (login.size() == 0)
			{
				cout << "Логин не может быть пустым. Повторите попытку" << endl;
				continue;
			}
			map <string, User> ::iterator it;
			it = users.find(login);
			if (it != users.end())
			{
				cout << "Пользователь с таким логином уже существует. Попробуйте еще раз." << endl;
				continue;
			}
			else
				break;
		}

		while (true)
		{
			password = "";
			cout << "Придумайте пароль (минимум 5 символов) - ";
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
			passwordCheck = "";
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

		Order cart;
		User user(name, login, password, users.size(), cart);
		ofstream out("users", ios::binary | ios::app);
		out << user;
		out.close();
		users.insert(make_pair(login, user));
		cout << "Регистрация прошла успешно.\nТеперь вы можете войти в свою учетную запись" << endl << endl;
		system("pause");
		return;
	}
}
