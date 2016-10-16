#include "stdafx.h"
#include "Vect.h"



Vector::Vector()
{
}


Vector::~Vector()
{
}

Vector Vector::operator+(Vector v) const
{
	if (v.size() != this->size()) throw "Sorry, can't add two vectors with different dimensions";
	Vector res = *this;
	for (int i = 0; i < res.size();i++)
	{
		res[i] += v[i];
	}
	return res;
}

Vector Vector::operator-(Vector v) const
{
	if (v.size() != this->size()) throw "Sorry, can't add two vectors with different dimensions";
	Vector res = *this;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] -= v[i];
	}
	return res;
}

Vector Vector::operator*(double d) const
{
	Vector res = *this;
	for (int i = 0; i < res.size(); i++)
	{
		res[i] *= d;
	}
	return res;
}

istream& Vector::operator>>(istream& is)
{
	for (int i = 0; i < this->size(); i++)
	{
		is >> (*this)[i];
	}
	return is;
}
