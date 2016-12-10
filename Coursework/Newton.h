#pragma once
#include "util.h"

class Newton
{
public:
	Newton();
	virtual void main() const;
	virtual ~Newton();
	Matrix<dfun2, 2, 1> F;
	Matrix<dfun2, 2, 2> W;
	bool debug;
	double eps;
};

