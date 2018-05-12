#include "stdafx.h"
#include "rpcTrafficLightDetectorVisibilityToggleFunctor.h"


rpcTrafficLightDetectorVisibilityToggleFunctor::rpcTrafficLightDetectorVisibilityToggleFunctor()
{
}

void rpcTrafficLightDetectorVisibilityToggleFunctor::operator()(raaTrafficLightUnit* pLight)
{
	pLight->toggleDetectionPointVisibility();
}

rpcTrafficLightDetectorVisibilityToggleFunctor::~rpcTrafficLightDetectorVisibilityToggleFunctor()
{
}
