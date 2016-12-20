#pragma once
#include "Newton.h"
#include <iostream>
#include <iomanip>

using namespace std;


template<int N>
class NewtonModified :
	public Newton<N>
{
public:
	NewtonModified(): debug(false), eps(0.001)
	{
	}

	virtual void main() const override
	{
		int k = 0;
		Matrix<double, N, 1> x1;
		Matrix<double, N, 1> x0 = this->x0;
		//cout << x0 << endl;
		Matrix<double, N, 1> F1 = -apply<N, 1>(this->F, x0);
		if (debug) {
			cout << F1 << endl;
			cout << endl;
		}
		Matrix<double, N, N> W1 = apply<N, N>(this->W, x0);
		auto W_lu = W1.fullPivLu();
		if (debug) {
			cout << W1 << endl;
			cout << endl;
			cout << W_lu.solve(F1) << endl << endl;
		}
		x1 = x0 + W1.fullPivLu().solve(F1);//x0 + deltaX
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
		wcout << L"| Модифікований метод Ньютона                                                                     |" << endl;
		cout << "#------------#------------------------------------#-----------------------------------------------#" << endl;
		wcout << L"| № Iтерації | Розв'язок (X)                      | Нев'язка(Z)                                   |" << endl;
		cout << "#------------#------------------------------------#-----------------------------------------------#" << endl;
		//wcout << L"# Ітерація " << k << ":\t\t\t#" << endl;
		stringstream ss;
		ss << x1.format(CleanFmt);
		string s = ss.str();
		ss.str("");
		ss.clear();
		ss << (apply<N, 1>(this->F, x1)).format(CleanFmt);
		string zs = ss.str();
		s.resize(34);
		zs.resize(45);
		cout << "|          0 | " << s << " | " << zs << " |" << endl;
		if (debug)
		{
			cout << delta<N>(x1, x0) << endl;
		}
		Matrix<double, N, 1> zero;
		for (int i = 0; i < N; i++)zero[i] = 0;
		//cout << endl;
		while (delta<N>(x1, x0) >= eps || delta<N>(apply<N, 1>(this->F, x1),zero ) >= eps)
		{
			/*cout << delta<N>(apply<N, 1>(this->F, x1), zero) << endl;*/
			k++;
			//wcout << L"Ітерація " << k << ":" << endl;
			x0 = x1;
			F1 = -apply<N, 1>(this->F, x0);
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
				wcout << L"Відносна похибка: " << relative_error << endl;
			}
			ss.str("");
			ss.clear();
			ss << x1.format(CleanFmt);
			s = ss.str();
			ss.str("");
			ss.clear();
			ss << (apply<N, 1>(this->F, x1)).format(CleanFmt);
			zs = ss.str();
			s.resize(34);
			zs.resize(45);
			cout << "| " << setw(10) << k <<" | " << s << " | " << zs << " |" << endl;
			if (debug) {
				cout << delta<N>(x1, x0) << endl << endl;
			}
			//cout << endl;
		}
		cout << "#------------#------------------------------------#-----------------------------------------------#" << endl;
		wcout << L"| Кінцевий результат: ";
		cout << s << "                                          |" << endl;
		cout << "#-------------------------------------------------------------------------------------------------#" << endl;
	}

	~NewtonModified()
	{
	}

	//Matrix<function<double(const Matrix<double, N, 1>&)>, N, 1> F;
	//Matrix<function<double(const Matrix<double, N, 1>&)>, N, N> W;
	//Matrix<double, N, 1> x0;
	bool debug;
	double eps;
};

