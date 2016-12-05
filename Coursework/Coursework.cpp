// Coursework.cpp : Defines the entry point for the console application.
//

// Vanilla includes:
#include "stdafx.h"
#include <iostream>

// In-project includes
#include "util.h"

// Namespaces
using namespace std;

// Main function
int main()
{
	cout << "Hello, world!" << endl;
	// TIP: there is a Dynamic predefined value for dynamic size
	Matrix<int, 3, 3> M;
	M << 1, 2, 3,
		4, 5, 6,
		7, 8, 9;
	cout << M << endl;
	Matrix<dfun, 3, 1> F;
	F << [](Vector3d x) {return x[0] + 4; },
		[](Vector3d x) {return x[1] * 4; },
		[](Vector3d x) {return x[2] - 4; };
	Vector3d v;
	v << 3.4, 5.6, 1.3;
	cout << apply(F,v) << endl;
	//VectorXd v(5);
	system("pause");
    return 0;
}

