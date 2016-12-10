#pragma once
#include "Newton.h"
class NewtonModified :
	public Newton
{
public:
	NewtonModified();
	virtual void main();
	~NewtonModified();
};

