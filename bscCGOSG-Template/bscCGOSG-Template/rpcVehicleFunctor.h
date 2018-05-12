#pragma once

#include "raaAnimatedComponent.h"
class rpcVehicleFunctor
{
public:
	rpcVehicleFunctor() {}

	virtual void operator()(raaAnimatedComponent *pVehicle) = 0;

	virtual ~rpcVehicleFunctor() {}
	
};

class rpcVehicleDetectionBoxVisibilityToggleFunctor : public rpcVehicleFunctor
{
public:
	rpcVehicleDetectionBoxVisibilityToggleFunctor() {}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		pVehicle->toggleDetectionBoxVisibility();
	}

	virtual ~rpcVehicleDetectionBoxVisibilityToggleFunctor() {}
};

class rpcAdjustVehicleSpeed : public rpcVehicleFunctor
{
public:
	rpcAdjustVehicleSpeed(double dMultiplier) : m_dMultiplier((dMultiplier)) {}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		//		printf("Vehicle speed (before): %f\n", pVehicle->getTimeMultiplier());
			pVehicle->setManualMultiplier(m_dMultiplier);
		//		printf("Vehicle speed (after): %f\n", pVehicle->getTimeMultiplier());
	}

	virtual ~rpcAdjustVehicleSpeed() {};

protected:
	double m_dMultiplier;
};

class rpcPauseVehicles : public rpcVehicleFunctor
{
public:
	rpcPauseVehicles(bool bIsPause)
	{
		m_bIsPause = bIsPause;
	}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		pVehicle->setPause(m_bIsPause);
	}

	virtual ~rpcPauseVehicles() {};
protected:
	bool m_bIsPause;
};
