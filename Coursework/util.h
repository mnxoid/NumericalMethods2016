#pragma once

// Eigen or boost includes:
#include <Eigen/Core>
#include <Eigen/Dense>

// Namespaces
using namespace Eigen;

// Type definitions
typedef std::function<double(const Vector2d&)> dfun2;
typedef std::function<double(const Vector3d&)> dfun3;


// Function definitions
template <int n, int k>
Matrix<double, n, k> apply(const Matrix<std::function<double(const Matrix<double, n, 1>&)>, n, k> &F, const Matrix<double, n, 1> &x)
{
	Matrix<double, n, k> ret;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			ret(i,j) = F(i,j)(x);
		}
	}
	return ret;
}

template <int n>
double delta(const Matrix<double, n, 1> &x1, const Matrix<double, n, 1> &x0)
{
	double maxdiff = 0;
	for (int i = 0; i < n; i++)
	{
		double delta = abs(x1[i] - x0[i]);
		if (delta > maxdiff) maxdiff = delta;
	}
	return maxdiff;
}
