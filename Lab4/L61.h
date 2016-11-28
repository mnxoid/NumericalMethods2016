#pragma once
#include "L60.h"
class L61 :
	public L60
{
private:
	virtual void coef(long double* a, long double* d, long double* f) override;

	virtual void progonka(long double* a, long double* b, long double* c, long double* f) override;

	virtual void cds(long double* a, long double* b, long double* c, long double* d, long double* f, long double* y, long double* y2) override;

public:
	/* Текст головної програми розв’язування заданої задачі
	(див. контрольний приклад) */
	virtual void main() override;

	L61();
	~L61();
};

