#pragma once
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

class Vector :
	public vector<double>
{
public:
	Vector();
	explicit Vector(int cap);
	Vector(initializer_list<double> l);
	Vector(const Vector& v);
	~Vector();
	Vector operator+(Vector v) const;
	Vector operator-(Vector v) const;
	//Vector operator*(double d) const;
	friend Vector operator+(double d, Vector v);
	friend Vector operator+(Vector v, double d);
	friend Vector operator-(double d, Vector v);
	friend Vector operator-(Vector v, double d);
	friend Vector operator*(double d, Vector v);
	friend Vector operator*(Vector v, double d);
	friend Vector operator/(double d, Vector v);
	friend Vector operator/(Vector v, double d);
	//Vector operator/(double d) const;
	friend istream& operator>>(istream& is, Vector v);
	friend ostream& operator<<(ostream& is, Vector v);
	Vector abs() const;
	double max();
};

double abs(Vector v);

