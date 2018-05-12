#pragma once

#include "rpcVehicleFunctor.h"

class rpcVehicleDetectionBoxVisibilityToggleFunctor: public rpcVehicleFunctor
{
public:
	rpcVehicleDetectionBoxVisibilityToggleFunctor();

	void operator()(raaAnimatedComponent *pVehicle) override;

	virtual ~rpcVehicleDetectionBoxVisibilityToggleFunctor();
};

