#define _CRT_SECURE_NO_WARNINGS
#include "Order.h"
void returnTime(int& day, int& month, int& year);
int input();


Order::Order()
{
	this->saleFull = 0;
	this->ID = 0;
	this->summaFull = 0;
	this->cart = {};
}

void Order::updateFlag(map <string, FlowerAdmin>& flowers)
{
	map <string, FlowerAdmin>::iterator it;

	for (int i = 0; i < cart.size(); i++)
	{
		it = flowers.find(cart[i].returnName());
		if (it == flowers.end())
		{
			cart[i].setFlag(false);
		}
		else if (it->second.returnCount() < cart[i].returnCount())
			cart[i].setFlag(false);
	}
}

ofstream& operator<<(ofstream& out, const Order& order)
{
	out.write((char*)&order.ID, sizeof(order.ID));
	out.write((char*)&order.summaFull, sizeof(order.summaFull));
	out.write((char*)&order.saleFull, sizeof(order.saleFull));

	int size = order.cart.size();
	out.write((char*)&size, sizeof(size));

	for (int i = 0; i < size; i++)
	{
		out << order.cart[i];
	}

	return out;
}

ifstream& operator>>(ifstream& in, Order& order)
{
	in.read((char*)&order.ID, sizeof(order.ID));
	if (in.eof())
		return in;
	in.read((char*)&order.summaFull, sizeof(order.summaFull));
	in.read((char*)&order.saleFull, sizeof(order.saleFull));

	int size;
	in.read((char*)&size, sizeof(size));

	FlowerOrder buf;
	for (int i = 0; i < size; i++)
	{
		in >> buf;
		order.cart.push_back(buf);
	}

	return in;
}

void workCart(Order& Cart, int ID)
{
	if (Cart.cart.size() == 0)
	{
		cout << endl << "Корзина пока пуста. Но вы можете добавить товары в соответствующем меню." << endl << endl;
		return;
	}
	int nameSize = 8;
	Cart.summaFull = 0;
	Cart.saleFull = 0;
	for (int i = 0; i < Cart.cart.size(); i++)
	{
		nameSize = max(nameSize, (int)(Cart.cart[i].name.size()));
		Cart.summaFull += Cart.cart[i].price * Cart.cart[i].count - Cart.cart[i].price * Cart.cart[i].count * Cart.cart[i].sale;
		Cart.saleFull += Cart.cart[i].price * Cart.cart[i].count * Cart.cart[i].sale;
	}

	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|" << setfill(' ') << endl;
	cout << "|    | " << setw(nameSize) << "Название" << " | Стоимость | Количество | Скидка (руб.) | Цена со скидкой |" << endl;
	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|" << setfill(' ') << endl;

	for (int i = 0; i < Cart.cart.size(); i++)
	{
		cout << "| " << i + 1 << ". | " << setfill(' ') << setw(nameSize) << Cart.cart[i].name << " | "
			<< fixed << setprecision(2) << setw(9) << Cart.cart[i].price << " | "
			<< setw(10) << Cart.cart[i].count << " | "
			<< fixed << setprecision(2) << setw(13) << Cart.cart[i].price * Cart.cart[i].count * Cart.cart[i].sale << " | "
			<< fixed << setprecision(2) << setw(15) << Cart.cart[i].price * Cart.cart[i].count - Cart.cart[i].price * Cart.cart[i].count * Cart.cart[i].sale << " |" << endl;
		cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|" << setfill(' ') << endl;
	}

	cout << endl;
	cout << "| Позиций";
	cout.setf(ios::right);
	cout.width(57 + nameSize);
	cout << Cart.cart.size() << " |" << endl;
	cout.unsetf(ios::right);

	cout << "| Скидка";
	cout.setf(ios::right);
	cout.width(58 + nameSize);
	cout << Cart.saleFull << " |" << endl;
	cout.unsetf(ios::right);

	cout << "| Итого к оплате";
	cout.setf(ios::right);
	cout.width(50 + nameSize);
	cout << Cart.summaFull << " |" << endl;
	cout.unsetf(ios::right);

	cout << endl << endl;
	while (true)
	{
		cout << "Вы хотите оформить заказ или продолжить покупки? (1 - заказать, 2 - продолжить) - ";
		int choice = input();
		if (choice < 1 || choice > 2)
		{
			cout << "Введите 1 или 2" << endl << endl;
			continue;
		}
		switch (choice)
		{
		case 1:
		{
			Check check(Cart.cart, Cart.summaFull, Cart.saleFull, ID, false);
			ofstream out("checks", ios::binary | ios::app);
			out << check;
			out.close();
			cout << endl << "Заказ успешно оформлен!" << endl << endl;
			Cart.cart.clear();
			break;
		}
		default:
			return;
		}
		break;
	}
}

ofstream& operator<<(ofstream& out, const Check& check)
{
	out.write((char*)&check.ID, sizeof(check.ID));
	out.write((char*)&check.summaFull, sizeof(check.summaFull));
	out.write((char*)&check.saleFull, sizeof(check.saleFull));
	out.write((char*)&check.day, sizeof(check.day));
	out.write((char*)&check.month, sizeof(check.month));
	out.write((char*)&check.year, sizeof(check.year));
	out.write((char*)&check.purchase, sizeof(check.purchase));

	int size = check.cart.size();
	out.write((char*)&size, sizeof(size));

	for (int i = 0; i < size; i++)
	{
		out << check.cart[i];
	}

	return out;
}

ifstream& operator>>(ifstream& in, Check& check)
{
	in.read((char*)&check.ID, sizeof(check.ID));
	if (in.eof())
		return in;
	in.read((char*)&check.summaFull, sizeof(check.summaFull));
	in.read((char*)&check.saleFull, sizeof(check.saleFull));
	in.read((char*)&check.day, sizeof(check.day));
	in.read((char*)&check.month, sizeof(check.month));
	in.read((char*)&check.year, sizeof(check.year));
	in.read((char*)&check.purchase, sizeof(check.purchase));

	int size;
	in.read((char*)&size, sizeof(size));

	FlowerOrder buf;
	for (int i = 0; i < size; i++)
	{
		in >> buf;
		check.cart.push_back(buf);
	}

	return in;
}

Check::Check()
{
	returnTime(this->day, this->month, this->year);
	this->purchase = false;
}

Check::Check(vector<FlowerOrder> cart, float summaFull, float saleFull, int ID, bool purchase)
{
	this->cart = cart;
	this->summaFull = summaFull;
	this->saleFull = saleFull;
	this->ID = ID;
	returnTime(this->day, this->month, this->year);
	this->purchase = purchase;
}

bool Check::returnPurchase()
{
	return this->purchase;
}

int Check::returnID()
{
	return this->ID;
}

void checkOut(Check& check, int number)
{
	int nameSize = 8;
	check.summaFull = 0;
	check.saleFull;
	for (int i = 0; i < check.cart.size(); i++)
	{
		nameSize = max(nameSize, (int)(check.cart[i].name.size()));
		check.summaFull += check.cart[i].price * check.cart[i].count;
		check.saleFull += check.cart[i].price * check.cart[i].count * check.cart[i].sale;
	}


	cout << "######" << endl;
	cout << "# " << number << ". #" << endl;
	cout << setfill('#') << setw(71 + nameSize) << "" << setfill(' ') << endl;
	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|  #" << setfill(' ') << endl;
	cout << "|    | " << setw(nameSize) << "Название" << " | Стоимость | Количество | Скидка (руб.) | Цена со скидкой |  #" << endl;
	cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|  #" << setfill(' ') << endl;

	for (int i = 0; i < check.cart.size(); i++)
	{
		cout << "| " << i + 1 << ". | " << setfill(' ') << setw(nameSize) << check.cart[i].name << " | "
			<< fixed << setprecision(2) << setw(9) << check.cart[i].price << " | "
			<< setw(10) << check.cart[i].count << " | "
			<< fixed << setprecision(2) << setw(13) << check.cart[i].price * check.cart[i].count * check.cart[i].sale << " | "
			<< fixed << setprecision(2) << setw(15) << check.cart[i].price * check.cart[i].count - check.cart[i].price * check.cart[i].count * check.cart[i].sale << " |  #" << endl;
		cout << "|----|-" << setfill('-') << setw(nameSize) << "" << "-|-----------|------------|---------------|-----------------|  #" << setfill(' ') << endl;
	}

	cout.setf(ios::right);
	cout << '|';
	cout.width(70 + nameSize);
	cout << " |  #";
	cout.unsetf(ios::right);

	cout << endl;
	cout << "| Позиций";
	cout.setf(ios::right);
	cout.width(57 + nameSize);
	cout << check.cart.size() << " |  #" << endl;
	cout.unsetf(ios::right);

	cout << "| Скидка";
	cout.setf(ios::right);
	cout.width(58 + nameSize);
	cout << check.saleFull << " |  #" << endl;
	cout.unsetf(ios::right);

	cout << "| Итого к оплате";
	cout.setf(ios::right);
	cout.width(50 + nameSize);
	cout << check.summaFull << " |  #" << endl;
	cout.unsetf(ios::right);

	cout << setfill('#') << setw(71 + nameSize) << "" << setfill(' ') << endl;
}