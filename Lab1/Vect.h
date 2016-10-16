#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Vector :
	public vector<double>
{
public:
	Vector();
	~Vector();
	Vector operator+(Vector v) const;
	Vector operator-(Vector v) const;
	Vector operator*(double d) const;
	istream& operator>>(istream& is);
};

