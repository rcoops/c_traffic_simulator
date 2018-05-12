#pragma once

#include "raaAnimatedComponent.h"
class rpcVehicleFunctor
{
public:
	rpcVehicleFunctor() {};

	virtual void operator()(raaAnimatedComponent *pVehicle) = 0;

	virtual ~rpcVehicleFunctor() {};
	
};

