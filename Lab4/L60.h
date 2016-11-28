#pragma once
#include <math.h>
#include <iostream>
#include <functional>

#include <fstream>
using namespace std;
#define nd 500

class L60
{
protected:

	int kflag, i, hn, j, halfn;
	long double xi, h, emax, e, df, dk, sqk, del, dk1, dk2, dq, dk3;

	virtual void coef(long double* a, long double* d, long double* f) = 0;

	virtual void progonka(long double* a, long double* b, long double* c, long double* f) = 0;

	virtual void cds(long double* a, long double* b, long double* c, long double* d, long double* f, long double* y, long double* y2) = 0;
public:
	int n, kl, kr;
	long double eps, mu1, mu2, beta1, beta2;
	/* Текст головної програми розв’язування заданої задачі
	(див. контрольний приклад) */
	virtual void main() = 0;
	function<long double(long double)> k,f,q;

	L60();
	virtual ~L60();
};

