#pragma once
#include "raaTrafficLightUnit.h"

class rpcTrafficLightFunctor
{
public:
	rpcTrafficLightFunctor() {}

	virtual void operator()(raaTrafficLightUnit *pLight) = 0;

	virtual ~rpcTrafficLightFunctor() {}
};

class rpcToggleLightState : public rpcTrafficLightFunctor
{
public:
	rpcToggleLightState(raaTrafficLightUnit::rpcTrafficLightState eLightState)
	{
		m_eLightState = eLightState;
	}

	void operator()(raaTrafficLightUnit *pLight) override
	{
		pLight->setManualState(m_eLightState);
	}

	virtual ~rpcToggleLightState() {}

protected:
	raaTrafficLightUnit::rpcTrafficLightState m_eLightState;
};

class rpcRevertToAutomaticState : public rpcTrafficLightFunctor
{
public:
	rpcRevertToAutomaticState() {}

	void operator()(raaTrafficLightUnit *pLight) override
	{
		pLight->turnOffManualState();
	}

	virtual ~rpcRevertToAutomaticState() {}
};

class rpcTrafficLightDetectorVisibilityToggleFunctor : public rpcTrafficLightFunctor
{
public:
	rpcTrafficLightDetectorVisibilityToggleFunctor() {}

	void operator()(raaTrafficLightUnit *pLight) override
	{
		pLight->toggleDetectionPointVisibility();
	}

	virtual ~rpcTrafficLightDetectorVisibilityToggleFunctor() {}
};
