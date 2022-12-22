#include "Account.h"

Account::Account()
{
	this->name = "";
	this->login = "";
	this->password = "";
	this->role = false;
}

Account::Account(string name, string login, string password, bool role)
{
	this->name = name;
	this->login = login;
	this->password = password;
	this->role = role;           //админ = true, работник склада = false
}

string Account::Edit(map <string, Account>& accounts)
{

	vector <string> menu = { "Что вы хотите отредактировать?",
													 "1 - Имя",
													 "2 - Логин" };

	while (true)
	{
		int choice = menu_helper(menu);
		cout << endl << "Сейчас этот аккаунт выглядит так: " << endl << *this << endl;

		switch (choice)
		{
		case 1:
		{
			string name;
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
					cout << "Имя слишком длинное. Пожалуйста, используйте не более 50 символов." << endl;
					continue;
				}
				else
					break;
			}

			this->name = name;
			cout << endl << "Теперь этот аккаунт выглядит так: " << endl << *this << endl;
			system("pause");
			break;
		}
		case 2:
		{
			string login;
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

			this->login = login;
			cout << endl << "Теперь этот аккаунт выглядит так: " << endl << *this << endl;
			system("pause");
			break;
		}
		default:
			return this->login;
		}
	}
}

ofstream& operator<<(ofstream& out, Account& acc)
{
	acc.password = encryptionPassword(acc.password);

	//ofstream out("accounts", ios::binary | ios::app);
	out.write((char*)&acc.role, sizeof(acc.role));        // запись роли

	size_t len;

	len = acc.name.length() + 1;                            // запись имени
	out.write((char*)&len, sizeof(len));
	out.write((char*)acc.name.c_str(), len);

	len = acc.login.length() + 1;                           // запись логина
	out.write((char*)&len, sizeof(len));
	out.write((char*)acc.login.c_str(), len);

	len = acc.password.length() + 1;                        // запись пароля
	out.write((char*)&len, sizeof(len));
	out.write((char*)acc.password.c_str(), len);

	//out.close();
	return out;
}

ostream& operator<<(ostream& ostream, Account& acc)
{
	ostream << "|----------------|-" << setfill('-') << setw(acc.name.size()) << "" << "-|-" << setfill('-') << setw(acc.login.size()) << "" << "-|" << endl;
	ostream << "| " << (acc.role ? "Администратор " : "Сотрудник маг.") << " | " << setw(acc.name.size()) << left << acc.name << " | " << setw(acc.login.size()) << acc.login << " |" << endl;
	ostream << "|----------------|-" << setfill('-') << setw(acc.name.size()) << "" << "-|-" << setfill('-') << setw(acc.login.size()) << "" << "-|" << endl;
	ostream << setfill(' ');
	return ostream;
}

Account Account::operator=(Account acc)
{
	string name = acc.name;
	string login = acc.login;
	string password = acc.password;
	bool role = acc.role;
	return Account(name, login, password, role);
}

bool Account::returnPassword(string password)
{
	password = encryptionPassword(password);

	if (this->password == password)
		return true;
	return false;
}

bool Account::returnRole()
{
	return this->role;
}

string Account::returnLogin()
{
	return this->login;
}

ofstream& operator<<(ofstream& out, User& user)
{
	user.password = encryptionPassword(user.password);

	out.write((char*)&user.ID, sizeof(user.ID));

	size_t len;

	len = user.name.length() + 1;                            // запись имени
	out.write((char*)&len, sizeof(len));
	out.write((char*)user.name.c_str(), len);

	len = user.login.length() + 1;                           // запись логина
	out.write((char*)&len, sizeof(len));
	out.write((char*)user.login.c_str(), len);

	len = user.password.length() + 1;                        // запись пароля
	out.write((char*)&len, sizeof(len));
	out.write((char*)user.password.c_str(), len);

	out << user.cart;

	return out;
}

string encryptionPassword(string password)
{
	string encryptedPassword = "";

	for (int i = 0; i < password.size(); i++)
		encryptedPassword += password[i] ^ i;

	return encryptedPassword;
}

void updateFileAccounts(map<string, Account>& accounts)
{
	ofstream out("accounts", ios::binary | ios::trunc);

	for (map <string, Account> ::iterator it = accounts.begin(); it != accounts.end(); it++)
	{
		it->second.password = encryptionPassword(it->second.password);
		out << it->second;
	}

	out.close();
	return;
}

User::User(string FIO, string login, string password, int ID, Order cart)
{
	this->name = FIO;
	this->login = login;
	this->password = password;
	this->ID = ID;
	this->cart = cart;
}

Order& User::returnCart()
{
	return this->cart;
}

int User::returnID()
{
	return this->ID;
}

void updateFileUsers(map<string, User>& users)
{
	ofstream out("users", ios::binary | ios::trunc);

	for (map <string, User> ::iterator it = users.begin(); it != users.end(); it++)
	{
		it->second.password = encryptionPassword(it->second.password);
		out << it->second;
	}

	out.close();
	return;
}

void updateUserCart(map<string, User>& users, map<string, FlowerAdmin>& flowers)
{
	for (map <string, User>::iterator it = users.begin(); it != users.end(); it++)
	{
		it->second.cart.updateFlag(flowers);
	}
}
