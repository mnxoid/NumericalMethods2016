// Coursework.cpp : Defines the entry point for the console application.
//

// Vanilla includes:
#include "stdafx.h"
#include <iostream>

// Eigen or boost includes:
#include <Eigen/Core>

// Namespaces
using namespace std;
using namespace Eigen;

int main()
{
	cout << "Hello, world!" << endl;
	// TIP: there is a Dynamic predefined value for dynamic size
	Matrix<int, 3, 3> M;
	M << 1, 2, 3,
		4, 5, 6,
		7, 8, 9;
	//VectorXd v(5);
	system("pause");
    return 0;
}

