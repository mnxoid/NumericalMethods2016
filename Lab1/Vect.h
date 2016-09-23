#pragma once
#include <vector>

using namespace std;

class Vector :
	public vector<double>
{
public:
	Vector();
	~Vector();
	Vector operator+(Vector v) const
	{
		if (v.size() != this->size()) throw "Sorry, can't add two vectors with different dimensions";
		Vector res = *this;
		for (int i = 0; i < res.size();i++)
		{
			res[i] += v[i];
		}
		return res;
	}
};

