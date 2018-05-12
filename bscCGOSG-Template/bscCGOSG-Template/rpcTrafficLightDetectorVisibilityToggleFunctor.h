#pragma once
#include "rpcTrafficLightFunctor.h"

class rpcTrafficLightDetectorVisibilityToggleFunctor: public rpcTrafficLightFunctor
{
public:
	rpcTrafficLightDetectorVisibilityToggleFunctor();

	void operator()(raaTrafficLightUnit *pLight) override;

	virtual ~rpcTrafficLightDetectorVisibilityToggleFunctor();
};

