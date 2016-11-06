// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Vect.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
typedef double num;

//Vector u(num t)
//{
//	Vector v = { 1,1,1 };
//	return v*(-exp(-4.0*t) + (exp(-1.0) + 4000.0) / exp(3.0))*exp(-t*(t - 4.0)) / 400.0;
//}

Vector f(num t, Vector u)
{
	Vector res{
		-2 * u[0] + 5 * u[1],
		-(1 - sin(t))*u[0] - u[1] + 3 * u[2],
		-u[0]+2*u[2]
	};
	return res;
}

bool debug = false;
bool debug_input = true;
bool intermed_out = false;

int main()
{
	try {
		/*if (debug)
		{
			Vector v = u(1);
			cout << v << endl;
		}*/
		// Part 1
		if (debug) cout << "Part 1" << endl;
		num t0, T, eps, tau0, eM;
		Vector y0(3);
		if(debug_input)
		{
			t0 = 0.0;
			T = 1.0;
			y0 = { 2,1,1 };
			eps = 0.0000001;
			tau0 = 1;
		} else
		{
			cout << "Input t0: "; cin >> t0;
			cout << "Input T: "; cin >> T;
			cout << "Input y0: "; cin >> y0;
			cout << "Input eps: "; cin >> eps;
			cout << "Input tau0: "; cin >> tau0;
		}
		eM = eps;
		// Part 2
		if (debug) cout << "Part 2" << endl;
		num t = t0;
		Vector y = y0;
		num tau = tau0;
		cout << "t=" << left << setw(6) << t << "\t" << "y=" << y << endl;//" u(t)=" << u(t) << " |y-u(t)|=" << abs(y - u(t)) << endl;
		// Part 3
		if (debug) cout << "Part 3" << endl;
		while (abs(T - t) >= eM)
		{
			// Part 4
			if (debug) cout << "Part 4" << endl;
			if (t + tau > T) tau = T - t;
			// Part 5
			if (debug) cout << "Part 5" << endl;
			Vector v = y;
			num t1 = t;
			Vector w;
		part6:
			// Part 6
			if (debug) cout << "Part 6" << endl;
			int kf = 0;
			while (kf < 3) {
				// Part 7
				if (debug) cout << "Part 7" << endl;
				Vector k1 = f(t, y);
				// Part 8
				if (debug) cout << "Part 8" << endl;
				Vector k2 = f(t + tau / 2.0, y + tau*k1 / 2.0);
				Vector k3 = f(t + tau / 2.0, y + tau*k2 / 2.0);
				Vector k4 = f(t + tau, y + tau*k3);
				y = y + tau*(k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
				/*if (intermed_out)
				{
					num diff = abs(y - u(t));
					cout << "t=" << t << "\ty=" << y << "\tu(t)=" << u(t) << "\t|y-u(t)|=" << diff << endl;
				}*/
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
			num E = (y - w).abs().max() / (15.0 * max(1.0, y.abs().max()));
			num tauH = 2 * tau*min(5.0, max(0.1, 0.9*pow(eps / E, 0.2)));
			// Part 12
			if (debug) cout << "Part 12" << endl;
			if (E <= eps)
			{
				t = t + tau;
				y = y + (y - w) / 15.0;
				//Vector precise = u(t);
				tau = tauH;
				//num diff = abs(y - u(t));
				cout << "t=" << left << setw(6) << t << "\t" << "y=" << y << endl;//"\tu(t)=" << u(t) << "\t|y-u(t)|=" << diff << endl;
				/*if (eM < diff)
					eM = diff;*/
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
		//cout << "eM=" << eM << endl;
		system("pause");
		return 0;
	}
	catch (const exception &e) {
		cerr << "ERROR: " << e.what() << endl;
		system("pause");
		return 0;
	}
}

