#include "stdafx.h"
#include "util.h"

Vector3d apply(Matrix<dfun, 3, 1> F, Vector3d x)
{
	Vector3d ret;
	for (int i = 0; i < 3; i++)
	{
		ret[i] = F[i](x);
	}
	return ret;
}
