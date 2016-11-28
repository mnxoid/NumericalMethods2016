#pragma once
#include "L60.h"
class L62 :
	public L60
{
private:
	void coef(long double* a, long double* d, long double* f) override;

	void progonka(long double* a, long double* b, long double* c, long double* f) override;

	void cds(long double* a, long double* b, long double* c, long double* d, long double* f, long double* y, long double* y2) override;
public:
	/* Текст головної програми розв’язування заданої задачі
	(див. контрольний приклад) */
	void main() override;
	L62();
	~L62();
};

