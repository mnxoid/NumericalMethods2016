// Coursework.cpp : Defines the entry point for the console application.
//

// Vanilla includes:
#include "stdafx.h"
#include <iostream>
#include <chrono>

// In-project includes
#include "util.h"
#include "Newton.h"
#include "NewtonModified.h"

// Namespaces
using namespace std;
using namespace std::chrono;


// Main function
int main()
{
	Matrix<dfun2, 2, 1> F; //!< The NES
	/*
		x_1   + x_2   = 3
		x_1^2 + x_2^2 = 9
	*/
	F << [](const Vector2d &x) {return x[0] + x[1] - 3; },
		[](const Vector2d &x) {return x[0] * x[0] + x[1] * x[1] - 9; };
	Matrix<dfun2, 2, 2> W; //!< The Jacobian
	/*
		1,    1
		2x_1, 2x_2
	*/
	W << [](const Vector2d &x) {return 1; }, [](const Vector2d &x) {return 1; },
		[](const Vector2d &x) {return 2 * x[0]; }, [](const Vector2d &x) {return 2 * x[1]; };
	Newton N;
	NewtonModified N1;
	for(Newton* n : {&N, static_cast<Newton*>(&N1)})
	{
		n->F = F;
		n->W = W;
		n->eps = 0.000000001;
		n->debug = false;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		n->main();
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(t2 - t1).count();
		cout << "Time elapsed: " << duration << "mcs" << endl;
	}
	system("pause");
    return 0;
}

