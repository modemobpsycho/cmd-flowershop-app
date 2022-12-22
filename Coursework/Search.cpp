#include "search.h"

template<class T>
T Search::search<T>::toLower(T s)
{
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] = s[i] - ('Z' - 'z');
		if (s[i] >= 'À' && s[i] <= 'ß')
			s[i] = s[i] - ('ß' - 'ÿ');
	}
	return s;
}