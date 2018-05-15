#include "stdafx.h"
#include "rpcVehicleFactory.h"
#include "rpcCarVeryon.h"
#include "rpcDumpTruck.h"
#include "rpcCarDelta.h"

rpcVehicleFactory::rpcVehicleFactory()
{
}

raaAnimatedComponent* rpcVehicleFactory::vehicleFactory(const vehicleType eVehicleType, rpcContextAwareAnimationPath *pAP)
{
	switch (eVehicleType)
	{
	case delta:
		return new rpcCarDelta(pAP);
	case truck:
		return new rpcDumpTruck(pAP);
	default:
		return new rpcCarVeryon(pAP);
	}
}

raaAnimatedComponent* rpcVehicleFactory::buildRandomVehicle(rpcContextAwareAnimationPath *pAP)
{
	return vehicleFactory(getRandomType(), pAP);
}

rpcVehicleFactory::vehicleType rpcVehicleFactory::getRandomType()
{
	unsigned int uiRandom = rand() % numberOfTypes;
	return static_cast<vehicleType>(uiRandom);
}


rpcVehicleFactory::~rpcVehicleFactory()
{
}
