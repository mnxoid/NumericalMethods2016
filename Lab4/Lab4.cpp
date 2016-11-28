// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "L62.h"
#include "L61.h"


int main()
{
	L61 part1;
	L62 part2;

	L60* pts[] = { &part1, &part2 };

	for (L60* pt : pts)
	{
		pt->k = [](long double x) {return x + 1; };
		pt->q = [](long double x) {return exp(x); };
		pt->f = [](long double x) {return exp(-x*x); };
		pt->n = 10;
		pt->eps = 0.01;
		pt->mu1 = 0.0;
		pt->mu2 = 0.0;
		pt->beta1 = 0.0;
		pt->beta2 = 1.0;

		pt->kl = 1;
		pt->kr = 1;
	}
	
	part1.main();
	part2.main();

    return 0;
}

