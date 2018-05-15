#pragma once
#include "raaAnimatedComponent.h"

class rpcVehicleFactory
{
public:

	// fudge to get number of types
	enum vehicleType {
		veryon, delta, truck, numberOfTypes
	};

	static raaAnimatedComponent* vehicleFactory(const vehicleType eVehicleType, rpcContextAwareAnimationPath *pAP);
	static raaAnimatedComponent* buildRandomVehicle(rpcContextAwareAnimationPath* pAP);
	static vehicleType getRandomType();

	~rpcVehicleFactory();
protected:
	rpcVehicleFactory();

};

