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
#include <iomanip>

// Namespaces
using namespace std;
using namespace chrono;

//static IOFormat CleanFmt(StreamPrecision, 0, ", ", ", ", "", "", "(", ")");

// Main function
int main()
{
	system("@chcp 65001 > NUL");
	setlocale(LC_ALL, "Ukrainian");
	{
		Matrix<dfun2, 2, 1> F; //!< Матриця функцій, що задають систему рівнянь(у вигляді f(x)=0)
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		wcout << L"| Задана система рівнянь  №1                                                                      |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		wcout << L"| x_1   + x_2   - 3 = 0                                                                           |" << endl;
		wcout << L"| x_1^2 + x_2^2 - 9 = 0                                                                           |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		F << [](const Vector2d &x) {return x[0] + x[1] - 3; },
			[](const Vector2d &x) {return x[0] * x[0] + x[1] * x[1] - 9; };
		Matrix<dfun2, 2, 2> W; //!< Якобіан цієї системи
							   /**
							   *	1,    1
							   *	2x_1, 2x_2
							   **/
		W << [](const Vector2d &x) {return 1; }, [](const Vector2d &x) {return 1; },
			[](const Vector2d &x) {return 2 * x[0]; }, [](const Vector2d &x) {return 2 * x[1]; };
		double eps = 0.000000001;//!< Точність, з якою проводяться обчислення
		Vector2d x0;
		x0 << 5.0, 2.0; //!< Початкове наближення
		wcout << L"| Точність обчислення:  " << setw(10) << eps << "                                                                |" << endl;
		stringstream ss;
		ss << x0.format(CleanFmt);
		string s = ss.str();
		s.resize(29);
		wcout << L"| Початкове наближення:      "; cout << s; wcout << L"                                        |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		cout << endl << endl;
		Newton<2> N;
		NewtonModified<2> N1;
		for (Newton<2>* n : { &N, static_cast<Newton<2>*>(&N1) })
		{
			n->F = F;
			n->W = W;
			n->eps = eps;
			n->x0 = x0;
			n->debug = false;
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			n->main();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(t2 - t1).count();
			wcout << L"| Час виконання: " << setw(7) << duration << L" мікросекунд                                                              |" << endl;
			cout << "#-------------------------------------------------------------------------------------------------#" << endl;
			cout << endl << endl;
		}
	}
	{
		Matrix<dfun3, 3, 1> F; //!< Матриця функцій, що задають систему рівнянь(у вигляді f(x)=0)
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		wcout << L"| Задана система рівнянь  №2                                                                      |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		wcout << L"|  x_1^2 +  x_2^2 +  x_3^2 - 1 = 0                                                                |" << endl;
		wcout << L"| 2x_1^2 +  x_2^2 - 4x_3       = 0                                                                |" << endl;
		wcout << L"| 3x_1^2 - 4x_2   +  x_3^2     = 0                                                                |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		F << [](const Vector3d &x) {return x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 1; },
			[](const Vector3d &x) {return 2 * x[0] * x[0] + x[1] * x[1] - 4 * x[2]; },
			[](const Vector3d &x) {return 3 * x[0] * x[0] - 4 * x[1] + x[2] * x[2]; };
		Matrix<dfun3, 3, 3> W; //!< Якобіан цієї системи
							   /**
							   *	2x_1,	2x_2,    2x_3
							   *	4x_1,	2x_2,	-4
							   *    6x_1,  -4,	     2x_3
							   **/
		W << [](const Vector3d &x) {return 2 * x[0]; }, [](const Vector3d &x) {return 2 * x[1]; }, [](const Vector3d &x) {return 2 * x[2]; },
			[](const Vector3d &x) {return 4 * x[0]; }, [](const Vector3d &x) {return 2 * x[1]; }, [](const Vector3d &x) {return -4; },
			[](const Vector3d &x) {return 6 * x[0]; }, [](const Vector3d &x) {return -4; }, [](const Vector3d &x) {return 2 * x[2]; };
		double eps = 0.000000001;//!< Точність, з якою проводяться обчислення
		Vector3d x0;
		x0 << 0.5, 0.5, 0.5; //!< Початкове наближення
		wcout << L"| Точність обчислення:  " << setw(10) << eps << "                                                                |" << endl;
		stringstream ss;
		ss << x0.format(CleanFmt);
		string s = ss.str();
		s.resize(29);
		wcout << L"| Початкове наближення:      "; cout << s; wcout << L"                                        |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		cout << endl << endl;
		Newton<3> N;
		NewtonModified<3> N1;
		for (Newton<3>* n : { &N, static_cast<Newton<3>*>(&N1) })
		{
			n->F = F;
			n->W = W;
			n->eps = eps;
			n->x0 = x0;
			n->debug = false;
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			n->main();
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(t2 - t1).count();
			wcout << L"| Час виконання: " << setw(7) << duration << L" мікросекунд                                                              |" << endl;
			cout << "#-------------------------------------------------------------------------------------------------#" << endl;
			cout << endl << endl;
		}
	}
	system("pause");
    return 0;
}

