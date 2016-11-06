#include "stdafx.h"
#include "Vect.h"
#include <iomanip>


Vector::Vector()
{
}

Vector::Vector(int cap):vector<double>(cap)
{
}

Vector::Vector(initializer_list<double> l)
{
	for(double d: l)
	{
		push_back(d);
	}
}

Vector::Vector(const Vector& v):vector<double>(v)
{
}

Vector::~Vector()
{
}

Vector Vector::operator+(Vector v) const
{
	if (v.size() != this->size()) throw runtime_error("Sorry, can't add two vectors with different dimensions");
	Vector res = *this;
	for (int i = 0; i < res.size();i++)
	{
		res[i] += v[i];
	}
	return res;
}

Vector Vector::operator-(Vector v) const
{
	if (v.size() != this->size()) throw runtime_error("Sorry, can't substract two vectors with different dimensions");
	Vector res = *this;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] -= v[i];
	}
	return res;
}

Vector Vector::abs() const
{
	Vector res = *this;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = ::abs(res[i]);
	}
	return res;
}

double Vector::max()
{
	double res = DBL_MIN;
	for (int i = 0; i < size(); i++)
	{
		if (res < at(i))res = at(i);
	}
	return res;
}

double abs(Vector v)
{
	double res = 0;
	for (int i = 0; i < v.size(); i++)
	{
		res += v[i]*v[i];
	}
	return sqrt(res);
}

//Vector Vector::operator*(double d) const
//{
//	Vector res = *this;
//	for (int i = 0; i < res.size(); i++)
//	{
//		res[i] *= d;
//	}
//	return res;
//}

//Vector Vector::operator/(double d) const
//{
//	Vector res = *this;
//	for (int i = 0; i < res.size(); i++)
//	{
//		res[i] /= d;
//	}
//	return res;
//}

Vector operator+(double d, Vector v)
{
	Vector res = v;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] += d;
	}
	return res;
}

Vector operator+(Vector v, double d)
{
	Vector res = v;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] += d;
	}
	return res;
}

Vector operator-(double d, Vector v)
{
	Vector res(v.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = d-v[i];
	}
	return res;
}

Vector operator-(Vector v, double d)
{
	Vector res(v.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = v[i]-d;
	}
	return res;
}

Vector operator*(double d, Vector v)
{
	Vector res = v;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] *= d;
	}
	return res;
}

Vector operator*(Vector v, double d)
{
	Vector res = v;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] *= d;
	}
	return res;
}

Vector operator/(double d, Vector v)
{
	Vector res(v.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = d / v[i];
	}
	return res;
}

Vector operator/(Vector v, double d)
{
	Vector res(v.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = v[i] / d;
	}
	return res;
}

istream& operator>>(istream& is, Vector v)
{
	for (int i = 0; i < v.size(); i++)
	{
		is >> v[i];
	}
	return is;
}

ostream& operator<<(ostream& os, Vector v)
{
	if(v.size()==0)
	{
		os << "{}";
		return os;
	}
	os << "{";
	for (int i = 0; i < v.size()-1; i++)
	{
		os << left << setw(10) << v[i] <<", ";
	}
	os << left << setw(10) << v[v.size() - 1] << "}";
	return os;
}