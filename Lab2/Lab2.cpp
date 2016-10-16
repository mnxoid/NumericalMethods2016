// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
using namespace std;
typedef double num;

num u(num t)
{
	return (-exp(-4.0*t) + (exp(-1.0) + 4000.0) / exp(3.0))*exp(-t*(t - 4.0)) / 400.0;
}

num f(num t, num u)
{
	return exp(-t*t)*0.01 - 2.0 * (t - 2.0)*u;
}

bool debug = false;
bool intermed_out = false;

int main()
{
	if (debug)
	{
		cout << u(1) << endl;
	}
	// Part 1
	if (debug) cout << "Part 1" << endl;
	num t0, T, y0, eps, tau0, eM;
	cout << "Input t0: "; cin >> t0;
	cout << "Input T: "; cin >> T;
	cout << "Input y0: "; cin >> y0;
	cout << "Input eps: "; cin >> eps;
	cout << "Input tau0: "; cin >> tau0;
	cout << "Input eM: "; cin >> eM;
	// Part 2
	if (debug) cout << "Part 2" << endl;
	num t = t0;
	num y = y0;
	num tau = tau0;
	cout << "t=" << t << " y=" << y << " u(t)=" << u(t) << " |y-u(t)|=" << abs(y - u(t)) << endl;
	// Part 3
	if (debug) cout << "Part 3" << endl;
	while (abs(T - t) >= eM)
	{
		// Part 4
		if (debug) cout << "Part 4" << endl;
		if (t + tau > T) tau = T - t;
		// Part 5
		if (debug) cout << "Part 5" << endl;
		num v = y;
		num t1 = t;
		num w;
	part6:
		// Part 6
		if (debug) cout << "Part 6" << endl;
		int kf = 0;
		while (kf < 3) {
			// Part 7
			if (debug) cout << "Part 7" << endl;
			num k1 = f(t, y);
			// Part 8
			if (debug) cout << "Part 8" << endl;
			num k2 = f(t + tau / 2.0, y + tau*k1 / 2.0);
			num k3 = f(t + tau / 2.0, y + tau*k2 / 2.0);
			num k4 = f(t + tau, y + tau*k3);
			y = y + tau*(k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
			if (intermed_out)
			{
				num diff = abs(y - u(t));
				cout << "t=" << t << "\ty=" << y << "\tu(t)=" << u(t) << "\t|y-u(t)|=" << diff << endl;
			}
			// Part 9
			if (debug) cout << "Part 9" << endl;
			if (kf == 0)
			{
				w = y;
				y = v;
				tau = tau / 2;
			}
			else if (kf == 1)
			{
				t = t + tau;
			}
			kf++;
		}
		// Part 11
		if (debug) cout << "Part 11" << endl;
		num E = abs(y - w) / (15.0 * max(1.0, abs(y)));
		num tauH = 2 * tau*min(5.0, max(0.1, 0.9*pow(eps / E, 0.2)));
		// Part 12
		if (debug) cout << "Part 12" << endl;
		if (E <= eps)
		{
			t = t + tau;
			y = y + (y - w) / 15.0;
			num precise = u(t);
			tau = tauH;
			num diff = abs(y - u(t));
			cout << "t=" << t << "\ty=" << y << "\tu(t)=" << u(t) << "\t|y-u(t)|=" << diff << endl;
			if (eM < diff)
				eM = diff;
			continue;
		}
		else
		{
			y = v;
			t = t1;
			tau = tauH;
			goto part6;
		}
		break;
	}
	// Part 13
	if (debug) cout << "Part 13" << endl;
	cout << "eM=" << eM << endl;
	system("pause");
	return 0;
}

