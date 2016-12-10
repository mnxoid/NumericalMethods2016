#include "stdafx.h"
#include "NewtonModified.h"
#include <iostream>

using namespace std;

static IOFormat CleanFmt(StreamPrecision, 0, ", ", ", ", "", "", "(", ")");

NewtonModified::NewtonModified()
{
}

void NewtonModified::main()
{
	int k = 0;
	Vector2d x0, x1;
	x0 << 1, 5; // Need to know how to choose this
				//cout << x0 << endl;
	Matrix<double, 2, 1> F1 = -apply<2, 1>(F, x0);
	if (debug) {
		cout << F1 << endl;
		cout << endl;
	}
	Matrix<double, 2, 2> W1 = apply<2, 2>(W, x0);
	auto W_lu = W1.fullPivLu();
	if (debug) {
		cout << W1 << endl;
		cout << endl;
		cout << W_lu.solve(F1) << endl << endl;
	}
	x1 = x0 + W1.fullPivLu().solve(F1);//x0 + deltaX
	cout << "Iteration " << k << ":" << endl;
	cout << x1.format(CleanFmt) << endl;
	if (debug)
	{
		cout << delta<2>(x1, x0) << endl;
	}
	cout << endl;
	while (delta<2>(x1, x0) >= eps)
	{
		k++;
		cout << "Iteration " << k << ":" << endl;
		x0 = x1;
		F1 = -apply<2, 1>(F, x0);
		if (debug) {
			cout << F1 << endl;
			cout << endl;
		}
		if (debug) {
			cout << W1 << endl;
			cout << endl;
			cout << W_lu.solve(F1) << endl << endl;
		}
		x1 = x0 + W_lu.solve(F1);//x0 + deltaX
		if (debug) {
			double relative_error = (W1*x1 - F1).norm() / F1.norm(); // norm() is L2 norm
			cout << "The relative error is: " << relative_error << endl;
		}
		cout << x1.format(CleanFmt) << endl;
		if (debug) {
			cout << delta<2>(x1, x0) << endl << endl;
		}
		cout << endl;
	}
	cout << "-----------FINAL RESULT:-----------" << endl;
	cout << x1.format(CleanFmt) << endl;
}

NewtonModified::~NewtonModified()
{
}
