#pragma once

// Eigen or boost includes:
#include <Eigen/Core>

// Namespaces
using namespace Eigen;

// Type definitions
typedef std::function<double(Vector3d)> dfun;

// Function definitions
Vector3d apply(Matrix<dfun, 3, 1> F, Vector3d x);
