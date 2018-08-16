#include<iostream>
using namespace std;

class Date
{
public:
	Date(int year, int month, int day)
		: _year(year)
		, _month(month)
		, _day(day)
	{
		if (!(year > 0 && month > 0 && month < 13 && day>0 && day <= GetDayOfMonth(year, month)))
		{
			_year = 1990;
			_month = 1;
			_day = 1;
		}
	}
	bool isleap(int year)
	{
		if ((0 == year % 4 && 0 != year % 100) || (year % 400 == 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int GetDayOfMonth(int year, int month)
	{
		int mon[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
		if (2 == month)
		{
			if (isleap(year))
			{
				return mon[2] += 1;
			}
		}
		return mon[month];
	}
	// 求当前日期第days天后是哪一天？ 
	Date operator+(int days)
	{
		Date tmp(*this);
		if (days < 0)
		{
			days = -days;
			return tmp - days;
		}
		tmp._day += days;
		while (tmp._day > GetDayOfMonth(tmp._year,tmp._month))
		{
			tmp._day -= GetDayOfMonth(tmp._year, tmp._month);
			tmp._month++;
			if (tmp._month > 12)
			{
				tmp._year++;
				tmp._month = 1;
			}
		}
		return tmp;
	}

	// 求当前日期第days天前是哪一天？ 
	Date operator-(int days)
	{
		Date tmp(*this);
		if (days < 0)
		{
			days = -days;
			return tmp + days;
		}

		tmp._day -= days;
		while (tmp._day < 0)
		{
			tmp._month--;
			if (tmp._month < 1)
			{
				tmp._year--;
				tmp._month = 12;
			}
			tmp._day += GetDayOfMonth(tmp._year, tmp._month);

		}
		return tmp;
	}

	// 求两个日期之间的差值 
	int operator-(const Date& d)
	{
		int count = 0;
		Date Mindate(*this);
		Date Maxdate(d);
		if (Mindate > Maxdate)
		{
			Maxdate = Mindate;
			Mindate = d;
		}
		while (Maxdate != Mindate)
		{
			Mindate++;
			count++;
		}
		return count;
	}

	// 前置++ 
	Date& operator++()
	{
		*this = *this + 1;
		return *this;
	}

	// 后置++ 
	Date operator++(int)
	{
		Date tmp(*this);
		*this = *this + 1;
		return tmp;
	}
	Date& operator--()
	{
		*this = *this - 1;
		return *this;
	}
	Date operator--(int)
	{
		Date tmp(*this);
		*this = *this - 1;
		return tmp;
	}


	// 判断两个日期是否相等 
	bool operator==(const Date& d)
	{
		if (_year == d._year &&
			_month == d._month &&
			_day == d._day)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 判断两个日期是否不等 
	bool operator!=(const Date& d)
	{
		if (*this == d)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool operator>(const Date& d)
	{
		if ((_year > d._year) ||
			(_year == d._year&&_month > d._month) ||
			(_year == d._year&&_month == d._month&&_day > d._day))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator>=(const Date& d)
	{
		if (*this > d || *this == d)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator<(const Date& d)
	{
		if (*this > d)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool operator<=(const Date& d)
	{
		if (*this >= d)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

private:
	int _year;
	int _month;
	int _day;
};

int main()
{
	Date d1(2018, 5, 6);
	Date d2(2026, 10, 25);
	cout << d2 - d1 << endl;
	d1++;
	d1--;
	--d1;
	++d1;
	//d1 - 999;
	//d1 + 100;
	//d1 - (-999);
	return 0;
}