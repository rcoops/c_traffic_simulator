#pragma once
#include "raaTrafficLightUnit.h"

class rpcTrafficLightFunctor
{
public:
	rpcTrafficLightFunctor() {};

	virtual void operator()(raaTrafficLightUnit *pLight) = 0;

	virtual ~rpcTrafficLightFunctor() {};
};

