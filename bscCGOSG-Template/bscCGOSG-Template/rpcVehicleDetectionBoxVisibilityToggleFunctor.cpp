#include "stdafx.h"
#include "rpcVehicleDetectionBoxVisibilityToggleFunctor.h"

rpcVehicleDetectionBoxVisibilityToggleFunctor::rpcVehicleDetectionBoxVisibilityToggleFunctor()
{
}

void rpcVehicleDetectionBoxVisibilityToggleFunctor::operator()(raaAnimatedComponent* pVehicle)
{
	pVehicle->toggleDetectionBoxVisibility();
}

rpcVehicleDetectionBoxVisibilityToggleFunctor::~rpcVehicleDetectionBoxVisibilityToggleFunctor()
{
}
